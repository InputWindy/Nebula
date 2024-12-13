# property:
#   ADDITIONAL_COMPILER_FEATURES
#
cmake_minimum_required(VERSION 3.0...3.17)

# module name
project(Core LANGUAGES CXX)

# collect source files
file(GLOB_RECURSE CORE_SOURCES ./*.cpp ./*.c)
file(GLOB_RECURSE CORE_HEADERS ./*.h ./*.hpp)
file(GLOB_RECURSE CORE_SHADERS ./*.glsl)

# visual studio solution file tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${CORE_SOURCES} 
${CORE_HEADERS} 
${CORE_SHADERS} 
)

# add compile files
add_library(Core SHARED
${CORE_SOURCES} 
${CORE_HEADERS} 
)

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# cpp version 
set(COMPILER_FEATURES 
    cxx_std_20
    ${ADDITIONAL_COMPILER_FEATURES}
)

target_compile_definitions(Core PRIVATE CORE_EXPORTS)
target_compile_features(Core PUBLIC ${COMPILER_FEATURES})