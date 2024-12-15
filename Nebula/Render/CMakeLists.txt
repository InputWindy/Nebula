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

set(TARGET_LINKS Core)
set(TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})

# 添加glfw模块
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG master
)

FetchContent_GetProperties(glfw)
if (NOT glfw_POPULATED)
    FetchContent_MakeAvailable(glfw)
else()
    message(STATUS "MyLibrary is already fetched.")
endif()

set_property(TARGET glfw PROPERTY FOLDER "Nebula/ThirdParty/GLFW3")
set_property(TARGET uninstall PROPERTY FOLDER "Nebula/ThirdParty/GLFW3")
set_property(TARGET update_mappings PROPERTY FOLDER "Nebula/ThirdParty/GLFW3")
target_include_directories(Render PUBLIC
  $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
)

# 添加Vk模块
if(NOT TARGET Vulkan)
    include_directories(${Vulkan_INCLUDE_DIRS})

    find_package(Vulkan REQUIRED FATAL_ERROR)

    list(APPEND TARGET_LINKS ${Vulkan_LIBRARIES})
    list(APPEND TARGET_INCLUDES ${Vulkan_INCLUDE_DIRS})

    message(STATUS "Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")

endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(Render PUBLIC 
	${TARGET_LINKS}
)

target_include_directories(Render PUBLIC 
	${TARGET_INCLUDES}
)

