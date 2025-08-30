#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTranslator>
#include <QSettings>
#include <QCloseEvent>
#include <QTabWidget>
#include <QToolBar>
#include <QStatusBar>
#include <QMenu>
#include <QAction>

#include "TaskWidget.h"
#include "TimeTrackerWidget.h"
#include "PomodoroWidget.h"
#include "StatisticsWidget.h"
#include "SettingsDialog.h"

#include "../core/TaskManager.h"
#include "../core/TimeTracker.h"
#include "../core/PomodoroTimer.h"
#include "../core/FileManager.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;
    void changeEvent(QEvent *event) override;

private slots:
    // Слоты для действий меню
    void onActionNewTask();
    void onActionEditTask();
    void onActionDeleteTask();
    void onActionStartTimer();
    void onActionPauseTimer();
    void onActionStopTimer();
    void onActionStartPomodoro();
    void onActionSettings();
    void onActionAbout();
    void onActionExit();
    
    // Слоты для переключения языка
    void setLanguageRussian();
    void setLanguageEnglish();
    
    // Слоты для переключения темы
    void setThemeLight();
    void setThemeDark();
    
    // Слот для обновления статусной строки
    void updateStatusBar();

private:
    // Настройка интерфейса
    void setupUi();
    void setupMenus();
    void setupToolbar();
    void setupStatusBar();
    void setupConnections();
    
    // Загрузка и сохранение данных
    void loadData();
    void saveData();
    
    // Изменение языка
    void changeLanguage(const QString &language);
    
    // Изменение темы
    void changeTheme(const QString &theme);
    
    // Компоненты интерфейса
    Ui::MainWindow *ui;
    QTabWidget *tabWidget;
    QToolBar *mainToolBar;
    QStatusBar *mainStatusBar;
    
    // Виджеты для вкладок
    TaskWidget *taskWidget;
    TimeTrackerWidget *timeTrackerWidget;
    PomodoroWidget *pomodoroWidget;
    StatisticsWidget *statisticsWidget;
    
    // Действия меню
    QAction *actionNewTask;
    QAction *actionEditTask;
    QAction *actionDeleteTask;
    QAction *actionStartTimer;
    QAction *actionPauseTimer;
    QAction *actionStopTimer;
    QAction *actionStartPomodoro;
    QAction *actionSettings;
    QAction *actionAbout;
    QAction *actionExit;
    QAction *actionRussian;
    QAction *actionEnglish;
    QAction *actionLightTheme;
    QAction *actionDarkTheme;
    
    // Основные объекты приложения
    TaskManager taskManager;
    TimeTracker timeTracker;
    PomodoroTimer pomodoroTimer;
    FileManager fileManager;
    
    // Настройки
    QSettings settings;
    
    // Переводчик
    QTranslator translator;
};

#endif // MAINWINDOW_H