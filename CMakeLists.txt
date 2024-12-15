#project settings
set(PROJECT_NAME Example2)
set(CMAKE_MIN_VERSION 3.17)
set(CPP_VERSION 20)

set(ADDITIONAL_REQUIRED_MODULES
    NewPlugins
    NewPlugins1
    NewPlugins2
    Geometry
    Physics
    Render
)

#=================generic project=====================#
function(generic_template_code project_name)
    # 设置源目录和目标目录
    set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Template/Project)
    set(DST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Project/${project_name})

    if(EXISTS ${DST_DIR})
    else()

        # 创建项目文件夹
        file(MAKE_DIRECTORY ${DST_DIR})

        # 递归查找所有 .in 文件
        file(GLOB_RECURSE IN_FILES "${SRC_DIR}/*.in")

        message(STATUS "Template Files: ${IN_FILES}")

        # 遍历每一个 .in 文件
        foreach(IN_FILE ${IN_FILES})
            # 获取相对路径
            file(RELATIVE_PATH REL_PATH ${SRC_DIR} ${IN_FILE})

            # 去掉 .in 后缀并生成目标文件路径
            string(REPLACE ".in" "" OUT_FILE ${REL_PATH})
            set(OUT_FILE_PATH "${DST_DIR}/${OUT_FILE}")

            # 确保目标目录存在
            get_filename_component(OUT_DIR ${OUT_FILE_PATH} DIRECTORY)
            file(MAKE_DIRECTORY ${OUT_DIR})

            # 生成配置文件
            configure_file(${IN_FILE} ${OUT_FILE_PATH} @ONLY)
        endforeach()

    endif()

    message(STATUS "Finished Generic Template Code for: ${project_name}")
endfunction()

function(generic_module MODULE_NAME)
    # 设置源目录和目标目录
    set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Template/Module)
    set(DST_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Nebula/${MODULE_NAME})

    set(UPPER_MODULE_NAME)
    string(TOUPPER ${MODULE_NAME} UPPER_MODULE_NAME)

    message(STATUS "UPPER_MODULE_NAME:${UPPER_MODULE_NAME}")

    if(EXISTS ${DST_DIR})
    else()

        # 创建项目文件夹
        file(MAKE_DIRECTORY ${DST_DIR})

        # 递归查找所有 .in 文件
        file(GLOB_RECURSE IN_FILES "${SRC_DIR}/*.in")

        message(STATUS "Template Files: ${IN_FILES}")

        # 遍历每一个 .in 文件
        foreach(IN_FILE ${IN_FILES})
            # 获取相对路径
            file(RELATIVE_PATH REL_PATH ${SRC_DIR} ${IN_FILE})

            # 去掉 .in 后缀并生成目标文件路径
            string(REPLACE ".in" "" OUT_FILE ${REL_PATH})

            if(${OUT_FILE} STREQUAL "CMakeLists.txt")
                set(OUT_FILE_PATH "${DST_DIR}/${OUT_FILE}")
            else()
                set(OUT_FILE_PATH "${DST_DIR}/${MODULE_NAME}${OUT_FILE}")
            endif()

            message(STATUS "OUT_FILE_PATH:${MODULE_NAME}${OUT_FILE}")

            # 确保目标目录存在
            get_filename_component(OUT_DIR ${OUT_FILE_PATH} DIRECTORY)
            file(MAKE_DIRECTORY ${OUT_DIR})

            # 生成配置文件
            configure_file(${IN_FILE} ${OUT_FILE_PATH} @ONLY)
        endforeach()

    endif()
endfunction()

cmake_minimum_required(VERSION 3.17)
project(${PROJECT_NAME})
set(CMAKE_CXX_STANDARD ${CPP_VERSION})

generic_template_code(${PROJECT_NAME})

# compile source files
file(GLOB_RECURSE PROJECT_SOURCES Project/${PROJECT_NAME}/*.hpp)
file(GLOB_RECURSE PROJECT_SOURCES Project/${PROJECT_NAME}/*.cpp)
file(GLOB_RECURSE PROJECT_SOURCES Project/${PROJECT_NAME}/*.c)
file(GLOB_RECURSE PROJECT_SHADERS 
    Project/${PROJECT_NAME}/*.glsl
    Project/${PROJECT_NAME}/*.vert
    Project/${PROJECT_NAME}/*.frag
    Project/${PROJECT_NAME}/*.comp
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
)
add_executable(${PROJECT_NAME} Project/${PROJECT_NAME}/main.cpp)

# 设置 .glsl 文件不参与编译
set_source_files_properties(${PROJECT_SHADERS} PROPERTIES HEADER_FILE_ONLY ON)
target_sources(${PROJECT_NAME} PUBLIC 
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_SHADERS}
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
include(FetchContent)

set(NEBULA_MODULES
    Core
    ${ADDITIONAL_REQUIRED_MODULES}
)

foreach(SUBMODULE ${NEBULA_MODULES})

    generic_module(${SUBMODULE})

    add_subdirectory(Nebula/${SUBMODULE})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${SUBMODULE})
    message(STATUS "${SUBMODULE}")

    # 定义拷贝命令
    add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD # 在 Engine 构建完成后执行
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:${SUBMODULE}>                # 源文件路径（.dll 文件）
            $<TARGET_FILE_DIR:${PROJECT_NAME}>            # 目标目录（MyApp 的 .exe 目录）
        COMMENT "Copying ${SUBMODULE}.dll to the ${PROJECT_NAME} directory"
    )

    set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "Project")
    set_property(TARGET ${SUBMODULE} PROPERTY FOLDER "Nebula")
endforeach()







