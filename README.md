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

## Getting Started

### Prerequisites
- C++17 compatible compiler
- CMake 3.10 or higher

### Building the Application

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

### Running the Application
```
./TimeManagementApp
```

## Usage Guide

### Main Menu
The application starts with a main menu offering access to all features:
1. Task Management
2. Time Tracking
3. Pomodoro Timer
4. Statistics
5. Settings
6. Exit

### Task Management
- **View Tasks**: Browse all, pending, or completed tasks
- **Add Task**: Create new tasks with title, description, priority, and deadline
- **Edit Task**: Modify existing task details
- **Delete Task**: Remove tasks from the system

### Time Tracking
- **Start Tracking**: Begin tracking time for a selected task
- **Pause/Resume**: Temporarily stop or continue time tracking
- **Stop Tracking**: End the tracking session and save the time to the task

### Pomodoro Timer
- **Start Pomodoro**: Begin a work session (default: 25 minutes)
- **Pause/Resume**: Temporarily stop or continue the current session
- **Check Status**: View current session type, remaining time, and progress

### Statistics
- **Task Statistics**: View completion rates and task distribution
- **Time Statistics**: Analyze time spent on different tasks
- **Pomodoro Statistics**: Track completed sessions and productivity patterns

### Settings
- **Pomodoro Settings**: Customize work duration, break duration, and session count
- **Save/Load Settings**: Persist your preferences between sessions

## Anti-Procrastination Techniques

This application implements several proven anti-procrastination strategies:

1. **Task Breakdown**: Break large tasks into smaller, manageable subtasks
2. **Priority Matrix**: Use the Eisenhower Matrix to focus on what matters most
3. **Pomodoro Technique**: Work in focused intervals with scheduled breaks
4. **Time Awareness**: Track time spent to increase accountability
5. **Progress Visualization**: See your accomplishments to stay motivated

## File Structure

```
time-management-app/
├── include/                 # Header files
│   ├── Task.h               # Task class definition
│   ├── TimeTracker.h        # Time tracking functionality
│   ├── PomodoroTimer.h      # Pomodoro timer implementation
│   ├── TaskManager.h        # Task management system
│   ├── FileManager.h        # Data persistence
│   └── UserInterface.h      # Console UI implementation
├── src/                     # Source files
│   ├── main.cpp             # Application entry point
│   ├── Task.cpp             # Task class implementation
│   ├── TimeTracker.cpp      # Time tracking implementation
│   ├── PomodoroTimer.cpp    # Pomodoro timer implementation
│   ├── TaskManager.cpp      # Task management implementation
│   ├── FileManager.cpp      # Data persistence implementation
│   └── UserInterface.cpp    # Console UI implementation
├── CMakeLists.txt           # CMake build configuration
└── README.md                # Project documentation
```

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- The Pomodoro Technique was developed by Francesco Cirillo
- The Eisenhower Matrix was inspired by Dwight D. Eisenhower's approach to productivity