cmake_minimum_required(VERSION 3.12)
project(shadercc)

# configure glslang
set(SKIP_GLSLANG_INSTALL OFF)

option(ENABLE_SPVREMAPPER "Enables building of SPVRemapper" ON)
option(ENABLE_OPT "Enables spirv-opt capability if present" ON)

add_subdirectory(external)
SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++0x -stdlib=libc++")

add_subdirectory(glslangValidator)
