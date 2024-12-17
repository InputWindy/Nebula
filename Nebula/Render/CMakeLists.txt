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

set(TARGET_LINKS Core Platform)
set(TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../Platform)


# 添加Vk模块
if(NOT TARGET Vulkan)
    include_directories(${Vulkan_INCLUDE_DIRS})

    find_package(Vulkan REQUIRED FATAL_ERROR)

    list(APPEND TARGET_LINKS ${Vulkan_LIBRARIES})
    list(APPEND TARGET_INCLUDES ${Vulkan_INCLUDE_DIRS})

    message(STATUS "Vulkan_LIBRARIES: ${Vulkan_LIBRARIES}")

endif()

#添加glad模块
if(NOT TARGET glad)
  set(glad_SOURCE_DIR ../ThirdParty/glad)
  add_library(
    glad
    ${glad_SOURCE_DIR}/src/glad.c
    ${glad_SOURCE_DIR}/include/glad/glad.h
    ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
  )
  target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
  set_property(TARGET glad PROPERTY FOLDER "Nebula/ThirdParty")

  list(APPEND TARGET_LINKS glad)
  list(APPEND TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/glad)
endif()

# 添加imgui界面库
if(NOT TARGET imgui)
    if(ENABLE_UI)
      include(../ThirdParty/imgui.cmake)
      set_property(TARGET imgui PROPERTY FOLDER "Nebula/ThirdParty")

      list(APPEND TARGET_LINKS imgui)
      list(APPEND TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/imgui)
    endif()
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(Render PUBLIC 
	${TARGET_LINKS}
)

target_include_directories(Render PUBLIC 
	${TARGET_INCLUDES}
)

