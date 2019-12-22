#pragma once

#include <string>

namespace shadercc
{
namespace intermediate {

enum class UniformType
{
  Texture
};
struct ReflectionData
{
  std::string name;
  int binding;
  int size;
  int offset;
};

}
} // namespace shadercc
