# property:
#   ADDITIONAL_COMPILER_FEATURES
#
cmake_minimum_required(VERSION 3.0...3.17)

# module name
project(Nebula LANGUAGES CXX)

# collect source files
file(GLOB_RECURSE NEBULA_SOURCES ./*.cpp ./*.c)
file(GLOB_RECURSE NEBULA_HEADERS ./*.h ./*.hpp)
file(GLOB_RECURSE NEBULA_SHADERS ./*.glsl)

list(FILTER NEBULA_SOURCES EXCLUDE REGEX ".*/ThirdParty/.*")
list(FILTER NEBULA_HEADERS EXCLUDE REGEX ".*/ThirdParty/.*")
list(FILTER NEBULA_SHADERS EXCLUDE REGEX ".*/ThirdParty/.*")

# visual studio solution file tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${NEBULA_SOURCES} 
${NEBULA_HEADERS} 
${NEBULA_SHADERS} 
)

# add compile files
add_library(Nebula SHARED
${NEBULA_SOURCES} 
${NEBULA_HEADERS} 
)

# cpp version 
set(COMPILER_FEATURES 
    cxx_std_20
    ${ADDITIONAL_COMPILER_FEATURES}
)

target_compile_definitions(Nebula PRIVATE NEBULA_EXPORTS)
target_compile_features(Nebula PUBLIC ${COMPILER_FEATURES})

# set .dll output path
target_include_directories(Nebula PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}ThirdParty
    ${CMAKE_CURRENT_SOURCE_DIR}
)
