cmake_minimum_required(VERSION 3.12)
project(shadercc)

# configure glslang
set(SKIP_GLSLANG_INSTALL OFF)

option(ENABLE_SPVREMAPPER "Enables building of SPVRemapper" ON)
option(ENABLE_OPT "Enables spirv-opt capability if present" ON)

set(MYROOTDIR ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(external)

if (NOT MSVC)
  set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")
endif()

add_subdirectory(glslangValidator)
