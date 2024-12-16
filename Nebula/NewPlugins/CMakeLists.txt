# property:
#   ADDITIONAL_COMPILER_FEATURES
#
cmake_minimum_required(VERSION 3.17)

# module name
project(NewPlugins)
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

# collect source files
file(GLOB_RECURSE NEWPLUGINS_SOURCES ./*.cpp ./*.c)
file(GLOB_RECURSE NEWPLUGINS_HEADERS ./*.h ./*.hpp)
file(GLOB_RECURSE NEWPLUGINS_SHADERS ./*.glsl)

# visual studio solution file tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${NEWPLUGINS_SOURCES} 
${NEWPLUGINS_HEADERS} 
${NEWPLUGINS_SHADERS} 
)

# add compile files
add_library(NewPlugins SHARED
${NEWPLUGINS_SOURCES} 
${NEWPLUGINS_HEADERS} 
)

target_compile_definitions(NewPlugins PRIVATE NEWPLUGINS_EXPORTS)

set(TARGET_LINKS Core)
set(TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})



include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(NewPlugins PUBLIC 
	${TARGET_LINKS}
)

target_include_directories(NewPlugins PUBLIC 
	${TARGET_INCLUDES}
)

