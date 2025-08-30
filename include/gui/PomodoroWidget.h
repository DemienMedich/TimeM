#ifndef POMODOROWIDGET_H
#define POMODOROWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QListWidget>
#include <QSpinBox>
#include <QGroupBox>
#include <QRadioButton>
#include <QSlider>
#include <QDial>

#include "../core/PomodoroTimer.h"

class PomodoroWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PomodoroWidget(PomodoroTimer *pomodoroTimer, QWidget *parent = nullptr);
    ~PomodoroWidget();

    // Публичные методы для управления таймером Помодоро
    void startPomodoro();
    void pausePomodoro();
    void stopPomodoro();
    void resetPomodoro();

signals:
    // Сигналы для уведомления об изменениях
    void pomodoroStarted();
    void pomodoroPaused();
    void pomodoroStopped();
    void pomodoroReset();
    void sessionCompleted();

private slots:
    // Слоты для обработки действий пользователя
    void onStartButtonClicked();
    void onPauseButtonClicked();
    void onStopButtonClicked();
    void onResetButtonClicked();
    
    // Слоты для настройки параметров Помодоро
    void onWorkDurationChanged(int value);
    void onBreakDurationChanged(int value);
    void onLongBreakDurationChanged(int value);
    void onSessionsBeforeLongBreakChanged(int value);
    
    // Слот для обновления отображения времени
    void updateTimeDisplay();
    
    // Слот для обработки завершения сессии
    void handleSessionComplete();

private:
    // Настройка интерфейса
    void setupUi();
    void setupConnections();
    
    // Вспомогательные методы
    void updateButtonStates();
    void updateSettingsDisplay();
    void updateSessionHistory();
    void showNotification(const QString &title, const QString &message);
    
    // Компоненты интерфейса
    QLabel *timerLabel;
    QLabel *sessionTypeLabel;
    QLabel *sessionCountLabel;
    QProgressBar *timerProgressBar;
    
    QPushButton *startButton;
    QPushButton *pauseButton;
    QPushButton *stopButton;
    QPushButton *resetButton;
    
    QGroupBox *settingsGroupBox;
    QSpinBox *workDurationSpinBox;
    QSpinBox *breakDurationSpinBox;
    QSpinBox *longBreakDurationSpinBox;
    QSpinBox *sessionsBeforeLongBreakSpinBox;
    
    QListWidget *sessionHistoryList;
    
    // Таймер для обновления отображения
    QTimer *displayTimer;
    
    // Ссылка на таймер Помодоро
    PomodoroTimer *pomodoroTimer;
    
    // Флаг для отслеживания состояния таймера
    bool isTimerRunning;
    
    // Текущая сессия для отображения прогресса
    int currentSessionDuration;
};

#endif // POMODOROWIDGET_H