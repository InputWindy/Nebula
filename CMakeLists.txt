#project settings
set(PROJECT_NAME Example2)
set(CMAKE_MIN_VERSION 3.17)
set(CPP_VERSION 20)

#=================generic project=====================#
function(generic_template_code project_name)
    # 设置源目录和目标目录
    set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Template)
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

function(generic_project project_name)
    # common config
    project(${project_name})
    set(CMAKE_CXX_STANDARD ${CPP_VERSION})

    # compile source files
    file(GLOB_RECURSE PROJECT_SOURCES Project/${project_name}/*.hpp Project/${project_name}/*.cpp Project/${project_name}/*.c)
    file(GLOB_RECURSE PROJECT_HEADERS Project/${project_name}/*.h)
    file(GLOB_RECURSE PROJECT_SHADERS
        Project/${project_name}/*.glsl
        Project/${project_name}/*.vert
        Project/${project_name}/*.frag
        Project/${project_name}/*.comp
    )
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_SHADERS}
    )

    add_executable(${PROJECT_NAME} Project/${project_name}/main.cpp)

    set_source_files_properties(${PROJECT_SHADERS} PROPERTIES HEADER_FILE_ONLY ON)
    target_sources(${project_name} PUBLIC
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_SHADERS}
    )

    set_property(TARGET ${project_name} PROPERTY FOLDER "Project")
    message(STATUS "Finished Generic Project for: ${project_name}")

endfunction()

function(generic_projects)
    set(PROJECT_LIST "")

    set(TARGET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Project)

    file(GLOB ALL_ENTRIES "${TARGET_DIR}/*")

    foreach(ENTRY ${ALL_ENTRIES})
        if(IS_DIRECTORY ${ENTRY})
            get_filename_component(FOLDER_NAME ${ENTRY} NAME)
            list(APPEND PROJECT_LIST ${FOLDER_NAME})
        endif()
    endforeach()

    cmake_minimum_required(VERSION ${CMAKE_MIN_VERSION})

    foreach(PROJECT ${PROJECT_LIST})
        generic_project(${PROJECT})
    endforeach(PROJECT ${PROJECT_LIST})

endfunction()

generic_template_code(${PROJECT_NAME})

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

generic_projects()

# link libs
set(LINK_TARGETS
	Core
)

foreach(SUBMODULE ${LINK_TARGETS})
    add_subdirectory(Nebula/${SUBMODULE})

    # add_custom_command(
    #     TARGET ${SUBMODULE} POST_BUILD
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different
    #         $<TARGET_FILE:${SUBMODULE}>
    #         $<TARGET_FILE_DIR:${PROJECT_NAME}>
    #     COMMENT "Copying ${ENGINE_MODULE}.dll to the ${PROJECT_NAME} directory"
    # )
    set_property(TARGET ${SUBMODULE} PROPERTY FOLDER "Nebula")
endforeach()

target_link_libraries(${PROJECT_NAME} PUBLIC ${LINK_TARGETS})

set(CMAKE_FOLDER "CMAKE")
# set_property(TARGET ${LINK_TARGETS} PROPERTY FOLDER "Nebula")



