# #project settings
# set(PROJECT_NAME xpydec)
# set(CMAKE_MIN_VERSION 3.17)
# set(CPP_VERSION 20)
# #macro
# set(MACRO_NAME
#     -DGLM_FORCE_LEFT_HANDED
#     -DNOMINMAX
# )

# #=================generic project=====================#
# cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
# project("${PROJECT_NAME}")
# set(CMAKE_CXX_STANDARD ${CPP_VERSION})

# # 创建项目文件夹
# if(EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
# else()
#     file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
#     file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/Shaders)
# endif()

# # 创建模板文件
# if(EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/main.cpp )
# else()

# # 设置源目录和目标目录
# set(SRC_DIR ${CMAKE_SOURCE_DIR}/Template)
# set(DST_DIR ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})

# # 递归查找所有 .in 文件
# file(GLOB_RECURSE IN_FILES "${SRC_DIR}/*.in")

# # 遍历每一个 .in 文件
# foreach(IN_FILE ${IN_FILES})
#     # 获取相对路径
#     file(RELATIVE_PATH REL_PATH ${SRC_DIR} ${IN_FILE})

#     # 去掉 .in 后缀并生成目标文件路径
#     string(REPLACE ".in" "" OUT_FILE ${REL_PATH})
#     set(OUT_FILE_PATH "${DST_DIR}/${OUT_FILE}")

#     # 确保目标目录存在
#     get_filename_component(OUT_DIR ${OUT_FILE_PATH} DIRECTORY)
#     file(MAKE_DIRECTORY ${OUT_DIR})

#     # 生成配置文件
#     configure_file(${IN_FILE} ${OUT_FILE_PATH} @ONLY)
# endforeach()
# endif()

# #compile source files
# file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_NAME}/*.cpp)
# file(GLOB_RECURSE PROJECT_HEADERS ${PROJECT_NAME}/*.h)
# file(GLOB_RECURSE PROJECT_SHADERS
#     ${PROJECT_NAME}/*.glsl
#     ${PROJECT_NAME}/*.vert
#     ${PROJECT_NAME}/*.frag
#     ${PROJECT_NAME}/*.comp
# )

# set(TARGET_SOURCES
#     ${PROJECT_SOURCES}
#     ${PROJECT_HEADERS}
# )

# add_executable(${PROJECT_NAME} ${PROJECT_NAME}/main.cpp)

# # 设置 .glsl 文件不参与编译
# set_source_files_properties(${PROJECT_SHADERS} PROPERTIES HEADER_FILE_ONLY ON)

# target_sources(${PROJECT_NAME} PUBLIC 
#     ${PROJECT_SOURCES}
#     ${PROJECT_HEADERS}
#     ${PROJECT_SHADERS}
# )

# source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
#     ${PROJECT_SOURCES}
#     ${PROJECT_HEADERS}
#     ${PROJECT_SHADERS}
# )

# target_link_libraries(${PROJECT_NAME})

# project settings
set(PROJECT_NAME xpydec)
set(CMAKE_MIN_VERSION 3.17)
set(CPP_VERSION 20)

#macro
set(MACRO_NAME
    -DGLM_FORCE_LEFT_HANDED
    -DNOMINMAX

    -DPROJECT_NAME="${PROJECT_NAME}"
)

#================= generic project =====================#
cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})
project("${PROJECT_NAME}")
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

# create project
if(EXISTS ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
else()
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME})
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/Shaders)

    # recursive collect .in file
    file(GLOB_RECURSE IN_FILES "${CMAKE_SOURCE_DIR}/Template/*.in")
    foreach(IN_FILE ${IN_FILES})
        file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR}/Template ${IN_FILE})

        string(REPLACE ".in" "" OUT_FILE ${REL_PATH})
        set(OUT_FILE_PATH "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/${OUT_FILE}")

        get_filename_component(OUT_DIR ${OUT_FILE_PATH} DIRECTORY)
        file(MAKE_DIRECTORY ${OUT_DIR})

        configure_file(${IN_FILE} ${OUT_FILE_PATH} @ONLY)
    endforeach()
endif()

add_executable(${PROJECT_NAME} ${PROJECT_NAME}/main.cpp)

# collect project files
file(GLOB_RECURSE PROJECT_SOURCES ${PROJECT_NAME}/*.cpp)
file(GLOB_RECURSE PROJECT_HEADERS ${PROJECT_NAME}/*.h)
file(GLOB_RECURSE PROJECT_SHADERS
    ${PROJECT_NAME}/*.glsl
    ${PROJECT_NAME}/*.vert
    ${PROJECT_NAME}/*.frag
    ${PROJECT_NAME}/*.comp
)

# add compile files
set_source_files_properties(${PROJECT_SHADERS} PROPERTIES HEADER_FILE_ONLY ON)

target_sources(${PROJECT_NAME} PUBLIC
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
)

target_link_libraries(${PROJECT_NAME})
