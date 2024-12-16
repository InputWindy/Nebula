## Pilot使用vulkan作为imgui的后端
#set(imgui_SOURCE_DIR_ ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/imgui)
#
#file(GLOB imgui_sources CONFIGURE_DEPENDS  "${imgui_SOURCE_DIR_}/*.cpp")
#file(GLOB imgui_impl CONFIGURE_DEPENDS
#"${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.cpp"
#"${imgui_SOURCE_DIR_}/backends/imgui_impl_glfw.h"
#"${imgui_SOURCE_DIR_}/backends/imgui_impl_vulkan.cpp"
#"${imgui_SOURCE_DIR_}/backends/imgui_impl_vulkan.h")
#add_library(imgui STATIC ${imgui_sources} ${imgui_impl})
#target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${imgui_SOURCE_DIR_}>)
#target_include_directories(imgui PUBLIC $<BUILD_INTERFACE:${vulkan_include}>)
#target_link_libraries(imgui PUBLIC glfw ${vulkan_lib})


## 一般用opengl
set(imgui_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/imgui)
set(imgui_SOURCE_DIR ${imgui_SRC})

file(GLOB imgui_SRC ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h)

message(STATUS "imgui_SRC:${CMAKE_CURRENT_SOURCE_DIR}/../ThirdParty/imgui")

add_library(
        imgui
        ${imgui_SRC}
)
# target_link_libraries(imgui PUBLIC glfw glad)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GLAD2")
target_include_directories(
        imgui
        PUBLIC
        ${imgui_SOURCE_DIR}
        # ${imgui_SOURCE_DIR}/backends
)