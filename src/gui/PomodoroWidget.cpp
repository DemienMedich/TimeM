#include "gui/PomodoroWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QStyle>
#include <QPainter>
#include <QApplication>
#include <QSpinBox>
#include <QFormLayout>
#include <QListWidget>
#include <QDateTime>

PomodoroWidget::PomodoroWidget(PomodoroTimer* pomodoroTimer, QWidget* parent)
    : QWidget(parent)
    , pomodoroTimer(pomodoroTimer)
    , startButton(new QPushButton(tr("Начать"), this))
    , pauseButton(new QPushButton(tr("Пауза"), this))
    , stopButton(new QPushButton(tr("Стоп"), this))
    , resetButton(new QPushButton(tr("Сброс"), this))
    , timeLabel(new QLabel("25:00", this))
    , sessionTypeLabel(new QLabel(tr("Рабочая сессия"), this))
    , sessionsCompletedLabel(new QLabel(tr("Выполнено сессий: 0"), this))
    , progressBar(new QProgressBar(this))
    , settingsButton(new QPushButton(tr("Настройки"), this))
    , updateTimer(new QTimer(this))
    , sessionHistoryList(new QListWidget(this))
{
    setupUi();
    setupConnections();
    updateButtonStates();
    
    // Запуск таймера обновления интерфейса
    updateTimer->start(1000); // Обновление каждую секунду
}

PomodoroWidget::~PomodoroWidget()
{
    updateTimer->stop();
}

void PomodoroWidget::setupUi()
{
    // Настройка шрифта для таймера
    QFont timerFont = timeLabel->font();
    timerFont.setPointSize(48);
    timerFont.setBold(true);
    timeLabel->setFont(timerFont);
    timeLabel->setAlignment(Qt::AlignCenter);
    
    // Настройка шрифта для типа сессии
    QFont sessionTypeFont = sessionTypeLabel->font();
    sessionTypeFont.setPointSize(16);
    sessionTypeLabel->setFont(sessionTypeFont);
    sessionTypeLabel->setAlignment(Qt::AlignCenter);
    
    // Настройка шрифта для счетчика сессий
    sessionsCompletedLabel->setAlignment(Qt::AlignCenter);
    
    // Создание компоновки
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Группа таймера
    QGroupBox* timerGroup = new QGroupBox(tr("Таймер Помодоро"), this);
    QVBoxLayout* timerLayout = new QVBoxLayout(timerGroup);
    timerLayout->addWidget(sessionTypeLabel);
    timerLayout->addWidget(timeLabel);
    timerLayout->addWidget(progressBar);
    timerLayout->addWidget(sessionsCompletedLabel);
    
    // Группа кнопок
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(pauseButton);
    buttonLayout->addWidget(stopButton);
    buttonLayout->addWidget(resetButton);
    timerLayout->addLayout(buttonLayout);
    
    // Группа истории сессий
    QGroupBox* historyGroup = new QGroupBox(tr("История сессий"), this);
    QVBoxLayout* historyLayout = new QVBoxLayout(historyGroup);
    historyLayout->addWidget(sessionHistoryList);
    
    // Добавление всех элементов в основную компоновку
    mainLayout->addWidget(timerGroup);
    mainLayout->addWidget(historyGroup);
    mainLayout->addWidget(settingsButton);
    
    setLayout(mainLayout);
}

void PomodoroWidget::setupConnections()
{
    // Соединение сигналов и слотов
    connect(startButton, &QPushButton::clicked, this, &PomodoroWidget::startPomodoro);
    connect(pauseButton, &QPushButton::clicked, this, &PomodoroWidget::pausePomodoro);
    connect(stopButton, &QPushButton::clicked, this, &PomodoroWidget::stopPomodoro);
    connect(resetButton, &QPushButton::clicked, this, &PomodoroWidget::resetPomodoro);
    connect(settingsButton, &QPushButton::clicked, this, &PomodoroWidget::showSettings);
    
    connect(updateTimer, &QTimer::timeout, this, &PomodoroWidget::updateTimerDisplay);
    
    // Соединение сигналов от PomodoroTimer
    connect(pomodoroTimer, &PomodoroTimer::sessionStarted, this, &PomodoroWidget::onSessionStarted);
    connect(pomodoroTimer, &PomodoroTimer::sessionCompleted, this, &PomodoroWidget::onSessionCompleted);
    connect(pomodoroTimer, &PomodoroTimer::sessionStatusChanged, this, &PomodoroWidget::updateButtonStates);
}

void PomodoroWidget::updateButtonStates()
{
    bool isRunning = pomodoroTimer->getIsRunning();
    bool isPaused = pomodoroTimer->getIsPaused();
    
    startButton->setEnabled(!isRunning || isPaused);
    pauseButton->setEnabled(isRunning && !isPaused);
    stopButton->setEnabled(isRunning);
    resetButton->setEnabled(!isRunning);
}

void PomodoroWidget::updateTimerDisplay()
{
    if (pomodoroTimer->getIsRunning() || pomodoroTimer->getIsPaused()) {
        // Обновление отображения времени
        std::string remainingTime = pomodoroTimer->getRemainingTimeString();
        timeLabel->setText(QString::fromStdString(remainingTime));
        
        // Обновление типа сессии
        std::string sessionType = pomodoroTimer->getCurrentSessionTypeString();
        QString sessionTypeText;
        
        if (sessionType == "work") {
            sessionTypeText = tr("Рабочая сессия");
            sessionTypeLabel->setStyleSheet("color: #d9534f;"); // Красный для рабочей сессии
        } else if (sessionType == "short_break") {
            sessionTypeText = tr("Перерыв");
            sessionTypeLabel->setStyleSheet("color: #5cb85c;"); // Зеленый для перерыва
        } else if (sessionType == "long_break") {
            sessionTypeText = tr("Длинный перерыв");
            sessionTypeLabel->setStyleSheet("color: #5bc0de;"); // Синий для длинного перерыва
        }
        
        sessionTypeLabel->setText(sessionTypeText);
        
        // Обновление прогресс-бара
        int totalSeconds = pomodoroTimer->getCurrentSessionDuration() * 60;
        int remainingSeconds = pomodoroTimer->getRemainingSeconds();
        int elapsedSeconds = totalSeconds - remainingSeconds;
        
        int progress = (elapsedSeconds * 100) / totalSeconds;
        progressBar->setValue(progress);
        
        // Обновление счетчика сессий
        int completedSessions = pomodoroTimer->getCompletedSessions();
        sessionsCompletedLabel->setText(tr("Выполнено сессий: %1").arg(completedSessions));
    }
}

void PomodoroWidget::startPomodoro()
{
    if (pomodoroTimer->getIsPaused()) {
        pomodoroTimer->resumeSession();
    } else {
        pomodoroTimer->startSession();
    }
    
    updateButtonStates();
}

void PomodoroWidget::pausePomodoro()
{
    if (!pomodoroTimer->getIsRunning() || pomodoroTimer->getIsPaused()) {
        QMessageBox::warning(this, tr("Таймер не запущен"), tr("Нет активной сессии Помодоро для приостановки"));
        return;
    }
    
    pomodoroTimer->pauseSession();
    updateButtonStates();
}

void PomodoroWidget::stopPomodoro()
{
    if (!pomodoroTimer->getIsRunning()) {
        QMessageBox::warning(this, tr("Таймер не запущен"), tr("Нет активной сессии Помодоро для остановки"));
        return;
    }
    
    pomodoroTimer->stopSession();
    
    // Обновление интерфейса
    timeLabel->setText(QString("%1:00").arg(pomodoroTimer->getWorkDuration()));
    progressBar->setValue(0);
    updateButtonStates();
    
    QMessageBox::information(this, tr("Сессия остановлена"), tr("Сессия Помодоро остановлена."));
}

void PomodoroWidget::resetPomodoro()
{
    pomodoroTimer->resetTimer();
    
    // Обновление интерфейса
    timeLabel->setText(QString("%1:00").arg(pomodoroTimer->getWorkDuration()));
    sessionTypeLabel->setText(tr("Рабочая сессия"));
    sessionTypeLabel->setStyleSheet("color: #d9534f;");
    progressBar->setValue(0);
    sessionsCompletedLabel->setText(tr("Выполнено сессий: 0"));
    
    // Очистка истории сессий
    sessionHistoryList->clear();
    
    updateButtonStates();
}

void PomodoroWidget::showSettings()
{
    QDialog settingsDialog(this);
    settingsDialog.setWindowTitle(tr("Настройки Помодоро"));
    
    QFormLayout* formLayout = new QFormLayout(&settingsDialog);
    
    // Настройки продолжительности
    QSpinBox* workDurationSpinBox = new QSpinBox(&settingsDialog);
    workDurationSpinBox->setRange(1, 60);
    workDurationSpinBox->setValue(pomodoroTimer->getWorkDuration());
    workDurationSpinBox->setSuffix(tr(" мин"));
    formLayout->addRow(tr("Продолжительность работы (минуты):"), workDurationSpinBox);
    
    QSpinBox* breakDurationSpinBox = new QSpinBox(&settingsDialog);
    breakDurationSpinBox->setRange(1, 30);
    breakDurationSpinBox->setValue(pomodoroTimer->getBreakDuration());
    breakDurationSpinBox->setSuffix(tr(" мин"));
    formLayout->addRow(tr("Продолжительность перерыва (минуты):"), breakDurationSpinBox);
    
    QSpinBox* longBreakDurationSpinBox = new QSpinBox(&settingsDialog);
    longBreakDurationSpinBox->setRange(5, 60);
    longBreakDurationSpinBox->setValue(pomodoroTimer->getLongBreakDuration());
    longBreakDurationSpinBox->setSuffix(tr(" мин"));
    formLayout->addRow(tr("Продолжительность длинного перерыва (минуты):"), longBreakDurationSpinBox);
    
    QSpinBox* sessionsBeforeLongBreakSpinBox = new QSpinBox(&settingsDialog);
    sessionsBeforeLongBreakSpinBox->setRange(1, 10);
    sessionsBeforeLongBreakSpinBox->setValue(pomodoroTimer->getSessionsBeforeLongBreak());
    formLayout->addRow(tr("Сессий перед длинным перерывом:"), sessionsBeforeLongBreakSpinBox);
    
    // Кнопки
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &settingsDialog);
    formLayout->addRow(buttonBox);
    
    connect(buttonBox, &QDialogButtonBox::accepted, &settingsDialog, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, &settingsDialog, &QDialog::reject);
    
    if (settingsDialog.exec() == QDialog::Accepted) {
        // Применение настроек
        pomodoroTimer->setWorkDuration(workDurationSpinBox->value());
        pomodoroTimer->setBreakDuration(breakDurationSpinBox->value());
        pomodoroTimer->setLongBreakDuration(longBreakDurationSpinBox->value());
        pomodoroTimer->setSessionsBeforeLongBreak(sessionsBeforeLongBreakSpinBox->value());
        
        // Обновление интерфейса
        if (!pomodoroTimer->getIsRunning()) {
            timeLabel->setText(QString("%1:00").arg(pomodoroTimer->getWorkDuration()));
        }
    }
}

void PomodoroWidget::onSessionStarted(const std::string& sessionType)
{
    QString sessionTypeText;
    QString notificationTitle;
    QString notificationMessage;
    
    if (sessionType == "work") {
        sessionTypeText = tr("Рабочая сессия");
        notificationTitle = tr("Рабочая сессия начата");
        notificationMessage = tr("Время сосредоточиться! Рабочая сессия началась.");
    } else if (sessionType == "short_break") {
        sessionTypeText = tr("Перерыв");
        notificationTitle = tr("Перерыв начался");
        notificationMessage = tr("Время для короткого перерыва!");
    } else if (sessionType == "long_break") {
        sessionTypeText = tr("Длинный перерыв");
        notificationTitle = tr("Длинный перерыв начался");
        notificationMessage = tr("Время для длинного перерыва! Вы это заслужили.");
    }
    
    // Добавление записи в историю сессий
    QDateTime currentTime = QDateTime::currentDateTime();
    QString historyEntry = QString("%1 - %2").arg(currentTime.toString("HH:mm:ss")).arg(sessionTypeText);
    sessionHistoryList->addItem(historyEntry);
    
    // Прокрутка к последнему элементу
    sessionHistoryList->scrollToBottom();
    
    // Показ уведомления
    QMessageBox::information(this, notificationTitle, notificationMessage);
}

void PomodoroWidget::onSessionCompleted(const std::string& sessionType)
{
    QString sessionTypeText;
    QString notificationTitle;
    QString notificationMessage;
    
    if (sessionType == "work") {
        sessionTypeText = tr("Рабочая сессия");
        notificationTitle = tr("Рабочая сессия завершена");
        notificationMessage = tr("Отличная работа! Вы завершили рабочую сессию.");
    } else if (sessionType == "short_break" || sessionType == "long_break") {
        sessionTypeText = sessionType == "short_break" ? tr("Перерыв") : tr("Длинный перерыв");
        notificationTitle = tr("Перерыв завершен");
        notificationMessage = tr("Перерыв окончен. Готовы вернуться к работе?");
    }
    
    // Добавление записи в историю сессий
    QDateTime currentTime = QDateTime::currentDateTime();
    QString historyEntry = QString("%1 - %2 завершена").arg(currentTime.toString("HH:mm:ss")).arg(sessionTypeText);
    sessionHistoryList->addItem(historyEntry);
    
    // Прокрутка к последнему элементу
    sessionHistoryList->scrollToBottom();
    
    // Показ уведомления
    QMessageBox::information(this, notificationTitle, notificationMessage);
}