# Time Management and Anti-Procrastination Application - Usage Guide

## Getting Started

After building the application, you can run either the console or GUI version:

### Console Version
```
./TimeManagementApp
```

### GUI Version
```
./TimeManagementAppGUI
```

## Console Interface

### Main Menu

The console application starts with a main menu offering access to all features:

1. **Task Management** - Create and manage your tasks
2. **Time Tracking** - Track time spent on tasks
3. **Pomodoro Timer** - Use the Pomodoro technique to fight procrastination
4. **Statistics** - View your productivity statistics
5. **Settings** - Configure application settings
6. **Exit** - Close the application

### Task Management

#### View Tasks
- View all tasks, pending tasks, or completed tasks
- Tasks are displayed with their ID, title, priority, deadline, status, and time spent

#### Add New Task
1. Select "Add New Task" from the Task Management menu
2. Enter the task title
3. Optionally enter a description
4. Select a priority level (1-4):
   - 1: Urgent & Important
   - 2: Important, Not Urgent
   - 3: Urgent, Not Important
   - 4: Not Urgent & Not Important
5. Optionally set a deadline (format: YYYY-MM-DD HH:MM)

#### Edit Task
1. Select "Edit Task" from the Task Management menu
2. Enter the ID of the task you want to edit
3. Choose what to edit:
   - Title
   - Description
   - Priority
   - Deadline
   - Status (complete/pending)

#### Delete Task
1. Select "Delete Task" from the Task Management menu
2. Enter the ID of the task you want to delete
3. Confirm deletion

### Time Tracking

#### Start Tracking
1. Select "Start Tracking" from the Time Tracking menu
2. Choose a task to track by entering its ID
3. The timer will start running for that task

#### Pause/Resume Tracking
- Select "Pause Tracking" to temporarily stop the timer
- Select "Resume Tracking" to continue timing the current task

#### Stop Tracking
- Select "Stop Tracking" to end the timing session
- The time will be saved to the task's total time spent

### Pomodoro Timer

#### Start Pomodoro
1. Select "Start Pomodoro" from the Pomodoro menu
2. The timer will start with a work session (default: 25 minutes)
3. After the work session, it will switch to a break session (default: 5 minutes)
4. After completing a set number of sessions, you'll get a longer break (default: 15 minutes)

#### Pause/Resume Pomodoro
- Select "Pause Pomodoro" to temporarily stop the timer
- Select "Resume Pomodoro" to continue the current session

#### Check Status
- Select "Check Pomodoro Status" to see:
  - Current session type (work/break)
  - Remaining time
  - Number of completed sessions

### Statistics

#### Task Statistics
- View the total number of tasks
- See how many tasks are completed and pending
- Check your task completion rate

#### Time Statistics
- View the total time spent on all tasks
- See a breakdown of time spent on each task

#### Pomodoro Statistics
- View the number of completed Pomodoro sessions
- See your current Pomodoro settings

### Settings

#### Pomodoro Settings
Customize your Pomodoro experience:
1. Work Duration (default: 25 minutes)
2. Break Duration (default: 5 minutes)
3. Long Break Duration (default: 15 minutes)
4. Sessions Before Long Break (default: 4)

#### Save/Load Settings
- Save your current settings for future use
- Load previously saved settings

## GUI Interface

### Main Window

The GUI version features a tabbed interface with:
- **Tasks** tab for managing your task list
- **Time Tracking** tab for monitoring time spent on tasks
- **Pomodoro** tab for using the Pomodoro technique
- **Statistics** tab for analyzing your productivity

### Menu Bar

The menu bar provides access to all features:

- **File** menu:
  - New Task
  - Edit Task
  - Delete Task
  - Exit

- **Tools** menu:
  - Start Timer
  - Pause Timer
  - Stop Timer
  - Start Pomodoro

- **Settings** menu:
  - Language (English/Russian)
  - Theme (Light/Dark)
  - Settings dialog

- **Help** menu:
  - About

### Tasks Tab

#### Task List
- View all your tasks in a table format
- Sort tasks by clicking on column headers
- Filter tasks by status (All/Pending/Completed) or priority

#### Task Management
- **Add Task**: Click the "Add Task" button to create a new task
- **Edit Task**: Select a task and click "Edit" or double-click on a task
- **Delete Task**: Select a task and click "Delete"
- **Complete Task**: Select a task and click "Complete"

#### Task Colors
Tasks are color-coded by priority and status:
- Red: Urgent & Important
- Yellow: Important, Not Urgent
- Orange: Urgent, Not Important
- Gray: Not Urgent & Not Important
- Green: Completed tasks

### Time Tracking Tab

#### Select Task
- Choose a task from the dropdown menu
- Click "Start" to begin tracking time

#### Timer Controls
- **Start**: Begin tracking time for the selected task
- **Pause**: Temporarily stop the timer
- **Stop**: End the timing session and save the time

#### Visualization
- View time distribution chart showing how time is spent across tasks
- See history of time tracking sessions

### Pomodoro Tab

#### Timer Display
- Large timer showing remaining time
- Session type indicator (Work/Break/Long Break)
- Progress bar showing session progress

#### Controls
- **Start**: Begin a Pomodoro session
- **Pause**: Temporarily stop the timer
- **Stop**: End the current session
- **Reset**: Reset the Pomodoro counter

#### Settings
- Click the "Settings" button to customize:
  - Work duration
  - Break duration
  - Long break duration
  - Sessions before long break

#### Session History
- View a list of completed sessions with timestamps

### Statistics Tab

#### Overview
- Summary of task completion rates
- Total time spent on tasks
- Average time per task
- Pomodoro sessions completed

#### Task Statistics
- Pie chart showing task distribution by priority
- Bar chart showing task completion trends

#### Time Statistics
- Time spent per task visualization
- Productivity trend over time

#### Pomodoro Statistics
- Sessions completed per day
- Work/break time ratio

### Settings Dialog

#### General Settings
- **Language**: Choose between English and Russian
- **Theme**: Choose between Light and Dark
- **Notifications**: Enable/disable notifications
- **Sounds**: Enable/disable sound effects
- **System**: Start with system, minimize to tray

#### Pomodoro Settings
- Work Duration
- Break Duration
- Long Break Duration
- Sessions Before Long Break
- Auto-start breaks
- Auto-start work sessions

#### Data Settings
- Data directory location
- Auto-save options
- Backup and restore functionality

## Localization

The application supports both English and Russian languages:

### Switching Languages
1. Open the Settings menu
2. Select Language
3. Choose your preferred language
4. The interface will update immediately

## Anti-Procrastination Tips

1. **Break Down Tasks**: Use the task management system to break large tasks into smaller, manageable subtasks.

2. **Prioritize Effectively**: Use the Eisenhower Matrix (priority levels 1-4) to focus on what truly matters:
   - Priority 1 (Urgent & Important): Do these tasks immediately
   - Priority 2 (Important, Not Urgent): Schedule time for these tasks
   - Priority 3 (Urgent, Not Important): Delegate if possible
   - Priority 4 (Not Urgent & Not Important): Eliminate or minimize time spent

3. **Use the Pomodoro Technique**: Work in focused intervals with scheduled breaks to maintain productivity and prevent burnout.

4. **Track Your Time**: Being aware of how you spend your time increases accountability and helps identify productivity patterns.

5. **Review Your Progress**: Regularly check your statistics to celebrate achievements and identify areas for improvement.

6. **Maintain Consistency**: Use the application daily to build productive habits over time.

## Data Persistence

The application automatically saves your tasks and settings when you exit. When you restart the application, your previous data will be loaded automatically.

### Backup and Restore
- Use the Settings dialog to create backups of your data
- Restore from backups if needed