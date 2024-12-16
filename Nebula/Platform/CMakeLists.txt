# property:
#   ADDITIONAL_COMPILER_FEATURES
#
cmake_minimum_required(VERSION 3.17)

# module name
project(Platform)
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

# collect source files
file(GLOB_RECURSE PLATFORM_SOURCES ./*.cpp ./*.c)
file(GLOB_RECURSE PLATFORM_HEADERS ./*.h ./*.hpp)
file(GLOB_RECURSE PLATFORM_SHADERS ./*.glsl)

# visual studio solution file tree
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
${PLATFORM_SOURCES} 
${PLATFORM_HEADERS} 
${PLATFORM_SHADERS} 
)

# add compile files
add_library(Platform SHARED
${PLATFORM_SOURCES} 
${PLATFORM_HEADERS} 
)

target_compile_definitions(Platform PRIVATE PLATFORM_EXPORTS)

set(TARGET_LINKS Core)
set(TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})


# Ìí¼ÓglfwÄ£¿é
if(NOT TARGET glfw)
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

    list(APPEND TARGET_LINKS glfw)
    list(APPEND TARGET_INCLUDES $<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>)
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(Platform PUBLIC 
	${TARGET_LINKS}
)

target_include_directories(Platform PUBLIC 
	${TARGET_INCLUDES}
)

#macro
if(ENABLE_WINDOW)
    add_compile_definitions(ENABLE_WINDOW)
endif()

