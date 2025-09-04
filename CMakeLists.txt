cmake_minimum_required(VERSION 3.15)
project(TimeM)

set(CMAKE_CXX_STANDARD 17)

# Подключаем ImGui
set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/external/imgui)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

# Исправленный единый add_executable
add_executable(TimeM
    src/main_gui.cpp
    src/TaskManager.cpp
    src/PomodoroTimer.cpp
    ${IMGUI_SOURCES}
)

# Инклюды
target_include_directories(TimeM PRIVATE
    src
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
)

# Зависимости
find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(TimeM PRIVATE glfw glad::glad OpenGL::GL)