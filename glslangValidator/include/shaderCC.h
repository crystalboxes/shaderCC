#pragma once

#include <string>
#include <vector>
#include <map>

namespace shadercc {
  typedef const char* (*OpenIncludeCallback)(const char*);

  enum class ShaderLanguage {
    Glsl,
    None
  };

  enum class ShaderStage {
    Vertex, Pixel, Compute, None
  };

  struct SourceDesc {
    std::string source;
    std::string file_name;
    std::string entry_point;
    ShaderStage stage;
  };

  struct TargetDesc {
    ShaderLanguage language;
    std::string version; // "310"
  };

  struct ReflectionData {
    std::map<std::string, std::string> texture_name_mapping;

  };

  struct ResultDesc {
    bool has_error;
    std::string error;

    std::string output;
    ReflectionData reflection_data;
  };

  void set_open_include_callback(OpenIncludeCallback callback);
  void set_shader_workdir(const std::string& dir);
  ResultDesc compile(const SourceDesc &source, const TargetDesc& target);

} // namespace shadercc

