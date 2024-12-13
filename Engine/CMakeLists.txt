# property:
#   ADDITIONAL_COMPILER_FEATURES
#
cmake_minimum_required(VERSION 3.0...3.17)

# module name
project(Engine LANGUAGES CXX)

# collect source files
file(GLOB_RECURSE ENGINE_SOURCES ./*.cpp ./*.c)
file(GLOB_RECURSE ENGINE_HEADERS ./*.h ./*.hpp)
file(GLOB_RECURSE ENGINE_SHADERS ./*.glsl)

list(FILTER ENGINE_SOURCES EXCLUDE REGEX ".*/ThirdParty/.*")
list(FILTER ENGINE_HEADERS EXCLUDE REGEX ".*/ThirdParty/.*")
list(FILTER ENGINE_SHADERS EXCLUDE REGEX ".*/ThirdParty/.*")

# visual studio solution file tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${ENGINE_SOURCES} 
${ENGINE_HEADERS} 
${ENGINE_SHADERS} 
)

# add compile files
add_library(Engine SHARED
${ENGINE_SOURCES} 
${ENGINE_HEADERS} 
)

# cpp version 
set(COMPILER_FEATURES 
    cxx_std_20
    ${ADDITIONAL_COMPILER_FEATURES}
)

target_compile_definitions(Engine PRIVATE ENGINE_EXPORTS)
target_compile_features(Engine PUBLIC ${COMPILER_FEATURES})

# set .dll output path
target_include_directories(Engine PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}ThirdParty
    ${CMAKE_CURRENT_SOURCE_DIR}
)
