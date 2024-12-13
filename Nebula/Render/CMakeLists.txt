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

if(NOT TARGET glfw)
  option(GLFW_BUILD_EXAMPLES "" OFF)
  option(GLFW_BUILD_TESTS "" OFF)
  option(GLFW_BUILD_DOCS "" OFF)
  option(GLFW_INSTALL "" OFF)
  add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ThirParty/glfw)
  set_target_properties(glfw PROPERTIES FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/ThirParty/glfw)
  set_target_properties(update_mappings PROPERTIES FOLDER ${CMAKE_CURRENT_SOURCE_DIR}/ThirParty/glfw)

  list(APPEND TARGET_LINKS glfw)
  list(APPEND TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/ThirParty/glfw/include)

endif()

if(NOT TARGET glad)
  set(glad_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ThirParty/glad)
  add_library(
    glad
    ${glad_SOURCE_DIR}/src/glad.c
    ${glad_SOURCE_DIR}/include/glad/glad.h
    ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
  )
  target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
  set_target_properties(glad PROPERTIES FOLDER ${third_party_folder}/glad)

  list(APPEND TARGET_LINKS glad)
  list(APPEND TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}/ThirParty/glad/include)

endif()

if(NOT TARGET Vulkan)
    # 添加包查找路径
    include_directories(${Vulkan_INCLUDE_DIRS})

    # 查找包
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
