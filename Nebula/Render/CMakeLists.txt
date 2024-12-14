# property:
#   ADDITIONAL_COMPILER_FEATURES
#
cmake_minimum_required(VERSION 3.17)

# module name
project(Render)
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

# collect source files
file(GLOB_RECURSE RENDER_SOURCES ./*.cpp ./*.c)
file(GLOB_RECURSE RENDER_HEADERS ./*.h ./*.hpp)
file(GLOB_RECURSE RENDER_SHADERS ./*.glsl)

# visual studio solution file tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${RENDER_SOURCES} 
${RENDER_HEADERS} 
${RENDER_SHADERS} 
)

# add compile files
add_library(Render SHARED
${RENDER_SOURCES} 
${RENDER_HEADERS} 
)

target_compile_definitions(Render PRIVATE RENDER_EXPORTS)

set(TARGET_LINKS)
set(TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})

FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master # 指定使用的分支、标签或提交
)

FetchContent_MakeAvailable(glfw)
set_property(TARGET glfw PROPERTY FOLDER "Nebula/ThirdParty/GLFW3")
set_property(TARGET uninstall PROPERTY FOLDER "Nebula/ThirdParty/GLFW3")
set_property(TARGET update_mappings PROPERTY FOLDER "Nebula/ThirdParty/GLFW3")
target_include_directories(Render PUBLIC
  $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
)

if(NOT TARGET Vulkan)
    include_directories(${Vulkan_INCLUDE_DIRS})

    find_package(Vulkan REQUIRED FATAL_ERROR)

    list(APPEND TARGET_LINKS ${Vulkan_LIBRARIES})
    list(APPEND TARGET_INCLUDES ${Vulkan_INCLUDE_DIRS})

endif()

target_link_libraries(Render PUBLIC 
	${TARGET_LINKS}
)

target_include_directories(Render PUBLIC 
	${TARGET_INCLUDES}
)
