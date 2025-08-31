cmake_minimum_required(VERSION 3.10)
project(TimeManagementApp)

# Опция для сборки GUI
option(BUILD_GUI "Build the GUI component" ON)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(include)

# Common source files (без UI-зависимых файлов)
set(COMMON_SOURCES
    src/Task.cpp
    src/TimeTracker.cpp
    src/PomodoroTimer.cpp
    src/TaskManager.cpp
    src/FileManager.cpp
)

if(BUILD_GUI)
    message(STATUS "Building GUI component")
    
    # Настройка Qt
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_AUTOUIC ON)
    
    # Поиск Qt
    find_package(Qt5 COMPONENTS Widgets Core REQUIRED)
    
    # GUI source files
    set(GUI_SOURCES
        src/main.cpp
        src/UserInterface.cpp
        # Добавьте другие GUI-специфичные файлы здесь
    )
    
    # Create GUI executable
    add_executable(TimeManagementApp ${COMMON_SOURCES} ${GUI_SOURCES})
    
    # Link Qt libraries
    target_link_libraries(TimeManagementApp Qt5::Widgets Qt5::Core)
    
else()
    message(STATUS "Building without GUI component")
    
    # Console source files
    set(CONSOLE_SOURCES
        src/main_console.cpp  # Вам нужно создать этот файл
        src/ConsoleInterface.cpp  # И этот, если нужно
    )
    
    # Create console executable
    add_executable(TimeManagementApp ${COMMON_SOURCES} ${CONSOLE_SOURCES})
endif()

# Add compiler flags
if(MSVC)
    target_compile_options(TimeManagementApp PRIVATE /W4)
else()
    target_compile_options(TimeManagementApp PRIVATE -Wall -Wextra -pedantic)
endif()

# Install target
install(TARGETS TimeManagementApp
    RUNTIME DESTINATION bin
)