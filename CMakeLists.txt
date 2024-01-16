cmake_minimum_required(VERSION 3.14)

include(FetchContent)
FetchContent_Declare(
  IMGUI
  GIT_REPOSITORY  https://github.com/ocornut/imgui.git
  GIT_SHALLOW ON
)
FetchContent_MakeAvailable(IMGUI)

project(fltk-imgui)

find_package(OpenGL REQUIRED)
find_package(FLTK REQUIRED CONFIG)

set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SRCS ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)

add_executable(app main.cpp imgui_impl_fltk.cpp ${IMGUI_SRCS})
target_include_directories(app PRIVATE ${imgui_SOURCE_DIR} ${imgui_SOURCE_DIR}/backends)
target_link_libraries(app PRIVATE fltk fltk_gl OpenGL::OpenGL)