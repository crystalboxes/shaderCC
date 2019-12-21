
set(SOURCES StandAlone.cpp shaderCC.cpp)

add_library(shaderCC STATIC ${SOURCES})
set_property(TARGET shaderCC PROPERTY FOLDER tools)
glslang_set_link_args(shaderCC)

target_include_directories(shaderCC PUBLIC include PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/../external/glslang)


if(ENABLE_OPT)
  target_compile_definitions(shaderCC PRIVATE ENABLE_OPT=1)
endif()

set(LIBRARIES
    glslang
    SPIRV
    glslang-default-resource-limits
    spirv-cross-glsl
)

if(ENABLE_SPVREMAPPER)
    set(LIBRARIES ${LIBRARIES} SPVRemapper)
endif()

if(WIN32)
    set(LIBRARIES ${LIBRARIES} psapi)
elseif(UNIX)
    if(NOT ANDROID)
        set(LIBRARIES ${LIBRARIES} pthread)
    endif()
endif(WIN32)

target_link_libraries(shaderCC PRIVATE ${LIBRARIES})
target_include_directories(shaderCC PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../External>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/External>)


if(WIN32)
    source_group("Source" FILES ${SOURCES})
endif(WIN32)
