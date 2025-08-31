# Time Management and Anti-Procrastination Application - Usage Guide

## Getting Started

After building the application, you can run it using:

```
./TimeManagementApp
```

## Main Menu

The application starts with a main menu offering access to all features:

1. **Task Management** - Create and manage your tasks
2. **Time Tracking** - Track time spent on tasks
3. **Pomodoro Timer** - Use the Pomodoro technique to fight procrastination
4. **Statistics** - View your productivity statistics
5. **Settings** - Configure application settings
6. **Exit** - Close the application

## Task Management

### View Tasks
- View all tasks, pending tasks, or completed tasks
- Tasks are displayed with their ID, title, priority, deadline, status, and time spent

### Add New Task
1. Select "Add New Task" from the Task Management menu
2. Enter the task title
3. Optionally enter a description
4. Select a priority level (1-4):
   - 1: Urgent & Important
   - 2: Important, Not Urgent
   - 3: Urgent, Not Important
   - 4: Not Urgent & Not Important
5. Optionally set a deadline (format: YYYY-MM-DD HH:MM)

### Edit Task
1. Select "Edit Task" from the Task Management menu
2. Enter the ID of the task you want to edit
3. Choose what to edit:
   - Title
   - Description
   - Priority
   - Deadline
   - Status (complete/pending)

### Delete Task
1. Select "Delete Task" from the Task Management menu
2. Enter the ID of the task you want to delete
3. Confirm deletion

## Time Tracking

### Start Tracking
1. Select "Start Tracking" from the Time Tracking menu
2. Choose a task to track by entering its ID
3. The timer will start running for that task

### Pause/Resume Tracking
- Select "Pause Tracking" to temporarily stop the timer
- Select "Resume Tracking" to continue timing the current task

### Stop Tracking
- Select "Stop Tracking" to end the timing session
- The time will be saved to the task's total time spent

## Pomodoro Timer

### Start Pomodoro
1. Select "Start Pomodoro" from the Pomodoro menu
2. The timer will start with a work session (default: 25 minutes)
3. After the work session, it will switch to a break session (default: 5 minutes)
4. After completing a set number of sessions, you'll get a longer break (default: 15 minutes)

### Pause/Resume Pomodoro
- Select "Pause Pomodoro" to temporarily stop the timer
- Select "Resume Pomodoro" to continue the current session

### Check Status
- Select "Check Pomodoro Status" to see:
  - Current session type (work/break)
  - Remaining time
  - Number of completed sessions

## Statistics

### Task Statistics
- View the total number of tasks
- See how many tasks are completed and pending
- Check your task completion rate

### Time Statistics
- View the total time spent on all tasks
- See a breakdown of time spent on each task

### Pomodoro Statistics
- View the number of completed Pomodoro sessions
- See your current Pomodoro settings

## Settings

### Pomodoro Settings
Customize your Pomodoro experience:
1. Work Duration (default: 25 minutes)
2. Break Duration (default: 5 minutes)
3. Long Break Duration (default: 15 minutes)
4. Sessions Before Long Break (default: 4)

### Save/Load Settings
- Save your current settings for future use
- Load previously saved settings

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