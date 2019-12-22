#include "shaderCC.h"

#include "glslangValidator.h"
#include "spirv_glsl.hpp"

#include <vector>
#include <map>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./../glslang/Public/ShaderLang.h"

#include "reflectionData.h"
extern  std::map<shadercc::intermediate::UniformType, std::vector<shadercc::intermediate::ReflectionData>> gReflectionData;

extern glslang::TProgram gProgram;

namespace shadercc
{
struct ShaderCCParams
{
  OpenIncludeCallback open_include_callback = nullptr;
  std::string dir;
};
ShaderCCParams SP;

void set_open_include_callback(OpenIncludeCallback callback)
{
  SP.open_include_callback = callback;
}

void set_shader_workdir(const std::string &dir)
{
  // TODO check if dir exist
  SP.dir = dir;
}

ResultDesc make_error(const std::string &error_string)
{
  return {true, error_string, "", {}};
}

char *get_shader_staget_str(ShaderStage stage, int &error)
{
  switch (stage)
  {
  case ShaderStage::Vertex:
    return "shader.vert";
  case ShaderStage::Pixel:
    return "shader.frag";
  case ShaderStage::Compute:
    return "shader.comp";
  case ShaderStage::None:
    return "";
  }
}

ResultDesc compile(const SourceDesc &source_desc, const TargetDesc &target_desc)
{
  std::string source;
  if (source_desc.file_name.size())
  {
    auto sourcePath = "/Users/hypernigga/Desktop/Projects/stuff/crosscompile/data/hlsl.frag";
    // load sourcePath to string
    std::ifstream t(source_desc.file_name);
    std::stringstream buffer;
    buffer << t.rdbuf();
    source = buffer.str();
  }
  else
  {
    source = source_desc.source;
  }

  int error = 0;
  char *fakeSourceName = get_shader_staget_str(source_desc.stage, error);
  if (error)
  {
    return make_error("Invalid shader stage");
  }

  char *outSourceName = "test.spv";

  AddGlslangSource(fakeSourceName, source.c_str());

  char *args[] = {
    "glslangValidator",
    "-q",
    "--hlsl-offsets", "--hlsl-iomap", "--auto-map-bindings",
    "-e", const_cast<char *>(source_desc.entry_point.c_str()),
    "-o", outSourceName,
    "-V",
    "-D", fakeSourceName
  };

  size_t argc = sizeof(args) / sizeof(args[0]);
  int result = glslangValidator(argc, args);

  switch (result)
  {
  case ESuccess:
    break;
  default:
    return make_error("glslangValidator error");
    break;
  }

  auto resultData = GetCompiledSpv()[outSourceName];

  // Read SPIR-V from disk or similar.
  std::vector<uint32_t> spirv_binary = resultData;

  spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));
  glsl.build_dummy_sampler_for_combined_images();
  glsl.build_combined_image_samplers();

  // The SPIR-V is now parsed, and we can perform reflection on it.
  spirv_cross::ShaderResources resources = glsl.get_shader_resources();

  // Get all sampled images in the shader.
  for (auto &resource : resources.sampled_images)
  {
    unsigned set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
    unsigned binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
    printf("Image %s at set = %u, binding = %u\n", resource.name.c_str(), set, binding);

    // Modify the decoration to prepare it for GLSL.
    glsl.unset_decoration(resource.id, spv::DecorationDescriptorSet);

    // Some arbitrary remapping if we want.
    glsl.set_decoration(resource.id, spv::DecorationBinding, set * 16 + binding);
  }

  // Set some options.
  spirv_cross::CompilerGLSL::Options options;
  options.version = 310;
  options.es = true;
  glsl.set_common_options(options);

  // Compile to GLSL, ready to give to GL driver.
  std::string source2 = glsl.compile();
  ResultDesc result_ = {false,  "", source2, {}};
  
  {
    // make reflection data here
    auto sh = glsl.get_shader_resources();
    int index = 0;
    for (auto &image : sh.sampled_images) {
      std::string name = std::string("_") + std::to_string(image.id);
      for (auto &t : gReflectionData[shadercc::intermediate::UniformType::Texture]) {
        if (t.binding == index) {
          result_.reflection_data.texture_name_mapping[name] = t.name;
        }
      }
      index++;
    }
    
  }

  return result_;
}
} // namespace shadercc
