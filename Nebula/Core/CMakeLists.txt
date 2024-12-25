cmake_minimum_required(VERSION 3.17)

# module name
project(Core)
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

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

target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${REQUIRED_INCLUDES})

#add_compile_definitions(${REQUIRED_MOUDLE_NAME})
target_compile_definitions(Core PRIVATE CORE_EXPORTS)

#add_compile_definitions(${REQUIRED_MODULES})

set(TARGET_LINKS)
set(TARGET_INCLUDES ${CMAKE_CURRENT_SOURCE_DIR})

# 添加glm模块
if(NOT TARGET glm)
    FetchContent_Declare(
      glm
      GIT_REPOSITORY https://github.com/icaven/glm.git
      GIT_TAG master
    )

    FetchContent_GetProperties(glm)
    if (NOT glm_POPULATED)
        FetchContent_MakeAvailable(glm)
    else()
        message(STATUS "MyLibrary is already fetched.")
    endif()

    set_property(TARGET glm PROPERTY FOLDER "Nebula/ThirdParty")

    list(APPEND TARGET_LINKS glm)
    list(APPEND TARGET_INCLUDES $<TARGET_PROPERTY:glm,INTERFACE_INCLUDE_DIRECTORIES>)
endif()

# 添加argparse模块
if(NOT TARGET argparse)
    FetchContent_Declare(
      argparse
      GIT_REPOSITORY https://github.com/p-ranav/argparse.git
      GIT_TAG master
    )

    FetchContent_GetProperties(argparse)
    if (NOT argparse_POPULATED)
        FetchContent_MakeAvailable(argparse)
    else()
        message(STATUS "MyLibrary is already fetched.")
    endif()

    set_property(TARGET argparse PROPERTY FOLDER "Nebula/ThirdParty")

    list(APPEND TARGET_LINKS argparse)
    list(APPEND TARGET_INCLUDES $<TARGET_PROPERTY:argparse,INTERFACE_INCLUDE_DIRECTORIES>)
endif()



include_directories(${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(Core PUBLIC 
	${TARGET_LINKS}
)

target_include_directories(Core PUBLIC 
	${TARGET_INCLUDES}
)


