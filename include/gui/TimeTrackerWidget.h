#ifndef TIMETRACKERWIDGET_H
#define TIMETRACKERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QListWidget>
#include <QProgressBar>
#include <QChart>
#include <QPieSeries>
#include <QChartView>

#include "../core/TimeTracker.h"
#include "../core/TaskManager.h"

QT_CHARTS_USE_NAMESPACE

class TimeTrackerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimeTrackerWidget(TimeTracker *timeTracker, TaskManager *taskManager, QWidget *parent = nullptr);
    ~TimeTrackerWidget();

    // Публичные методы для управления таймером
    void startTimer();
    void pauseTimer();
    void stopTimer();
    void updateTaskList();

signals:
    // Сигналы для уведомления об изменениях
    void timerStarted(int taskId);
    void timerPaused();
    void timerStopped();

private slots:
    // Слоты для обработки действий пользователя
    void onStartButtonClicked();
    void onPauseButtonClicked();
    void onStopButtonClicked();
    void onTaskSelectionChanged();
    
    // Слот для обновления отображения времени
    void updateTimeDisplay();
    
    // Слот для обновления графика
    void updateTimeChart();

private:
    // Настройка интерфейса
    void setupUi();
    void setupConnections();
    
    // Вспомогательные методы
    void updateButtonStates();
    void createTimeChart();
    void updateHistoryList();
    
    // Компоненты интерфейса
    QComboBox *taskComboBox;
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QLabel *timeLabel;
    QLabel *currentTaskLabel;
    QProgressBar *sessionProgressBar;
    QListWidget *historyListWidget;
    QChartView *chartView;
    QChart *chart;
    QPieSeries *timeSeries;
    
    // Таймер для обновления отображения
    QTimer *displayTimer;
    
    // Ссылки на основные объекты
    TimeTracker *timeTracker;
    TaskManager *taskManager;
    
    // Флаг для отслеживания состояния таймера
    bool isTimerRunning;
};

#endif // TIMETRACKERWIDGET_H