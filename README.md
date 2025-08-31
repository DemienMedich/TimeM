# Time Management and Anti-Procrastination Application

A comprehensive C++ application designed to help users manage their time effectively and combat procrastination through task management, time tracking, and the Pomodoro technique.

## Features

### Task Management
- Create, edit, and delete tasks
- Set priority levels using the Eisenhower Matrix (Urgent/Important)
- Add deadlines to tasks
- Track task completion status
- Organize tasks with subtasks

### Time Tracking
- Track time spent on specific tasks
- Pause and resume time tracking
- View time statistics for each task
- Analyze productivity patterns

### Pomodoro Technique
- Customizable work and break durations
- Long break option after a set number of sessions
- Session tracking and statistics
- Visual timer display

### Statistics and Reporting
- View task completion rates
- Analyze time spent on different tasks
- Track Pomodoro session history
- Monitor productivity trends

### Data Persistence
- Save tasks and settings to files
- Load previous session data
- Automatic backup of data files

### GUI Interface (New!)
- Modern graphical user interface built with Qt
- Intuitive task management with visual indicators
- Interactive charts for statistics
- Visual Pomodoro timer with progress indication
- Dark and light themes
- Full Russian localization support

## Getting Started

### Prerequisites
- C++17 compatible compiler
- CMake 3.10 or higher
- Qt 5.15 or higher (for GUI version)

### Building the Application

For detailed build instructions, please refer to the [BUILD_INSTRUCTIONS.md](BUILD_INSTRUCTIONS.md) file.

#### Quick Start (Console Version)

1. Clone the repository
```
git clone https://github.com/yourusername/time-management-app.git
cd time-management-app
```

2. Create a build directory
```
mkdir build
cd build
```

3. Generate build files with CMake
```
cmake ..
```

4. Build the application
```
cmake --build .
```

#### Building the GUI Version

```
mkdir build
cd build
cmake .. -DBUILD_GUI=ON
cmake --build .
```

### Running the Application

#### Console Version
```
./TimeManagementApp
```

#### GUI Version
```
./TimeManagementAppGUI
```

## Usage Guide

For detailed usage instructions, please refer to the [USAGE_GUIDE.md](USAGE_GUIDE.md) file.

### Main Menu (Console Version)
The application starts with a main menu offering access to all features:
1. Task Management
2. Time Tracking
3. Pomodoro Timer
4. Statistics
5. Settings
6. Exit

### GUI Version
The GUI version provides a tabbed interface with:
- Tasks tab for managing your task list
- Time Tracking tab for monitoring time spent on tasks
- Pomodoro tab for using the Pomodoro technique
- Statistics tab for analyzing your productivity

## Localization

The application supports both English and Russian languages. You can switch languages in the Settings menu.

### Available Languages
- English (default)
- Russian (Русский)

## Anti-Procrastination Techniques

This application implements several proven anti-procrastination strategies:

1. **Task Breakdown**: Break large tasks into smaller, manageable subtasks
2. **Priority Matrix**: Use the Eisenhower Matrix to focus on what matters most
3. **Pomodoro Technique**: Work in focused intervals with scheduled breaks
4. **Time Awareness**: Track time spent to increase accountability
5. **Progress Visualization**: See your accomplishments to stay motivated

## File Structure

```
TimeManagementApp/
├── include/                 # Header files
│   ├── core/                # Core functionality
│   │   ├── Task.h
│   │   ├── TimeTracker.h
│   │   ├── PomodoroTimer.h
│   │   ├── TaskManager.h
│   │   └── FileManager.h
│   └── gui/                 # GUI components
│       ├── MainWindow.h
│       ├── TaskWidget.h
│       ├── TimeTrackerWidget.h
│       ├── PomodoroWidget.h
│       ├── StatisticsWidget.h
│       └── SettingsDialog.h
├── src/                     # Source files
│   ├── core/                # Core implementation
│   │   ├── Task.cpp
│   │   ├── TimeTracker.cpp
│   │   ├── PomodoroTimer.cpp
│   │   ├── TaskManager.cpp
│   │   └── FileManager.cpp
│   ├── gui/                 # GUI implementation
│   │   ├── MainWindow.cpp
│   │   ├── TaskWidget.cpp
│   │   ├── TimeTrackerWidget.cpp
│   │   ├── PomodoroWidget.cpp
│   │   ├── StatisticsWidget.cpp
│   │   └── SettingsDialog.cpp
│   ├── main.cpp             # Console app entry point
│   └── main_gui.cpp         # GUI app entry point
├── forms/                   # Qt Designer UI files
│   ├── mainwindow.ui
│   ├── taskwidget.ui
│   └── ...
├── resources/               # Application resources
│   ├── icons/               # Icons
│   ├── styles/              # CSS style sheets
│   └── translations/        # Localization files
│       ├── timemanager_ru.ts
│       └── timemanager_ru.qm
├── CMakeLists.txt           # CMake build configuration
├── README.md                # Project documentation
├── USAGE_GUIDE.md           # Usage instructions
└── BUILD_INSTRUCTIONS.md    # Build instructions
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- The Pomodoro Technique was developed by Francesco Cirillo
- The Eisenhower Matrix was inspired by Dwight D. Eisenhower's approach to productivity
- GUI built with Qt framework