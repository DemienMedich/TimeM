#include "gui/TimeTrackerWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QTimer>
#include <QChart>
#include <QPieSeries>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QDateTime>

using namespace QtCharts;

TimeTrackerWidget::TimeTrackerWidget(TimeTracker* timeTracker, TaskManager* taskManager, QWidget* parent)
    : QWidget(parent)
    , timeTracker(timeTracker)
    , taskManager(taskManager)
    , taskComboBox(new QComboBox(this))
    , startButton(new QPushButton(tr("Начать"), this))
    , pauseButton(new QPushButton(tr("Пауза"), this))
    , stopButton(new QPushButton(tr("Стоп"), this))
    , currentTaskLabel(new QLabel(tr("Текущая задача:"), this))
    , currentTaskValueLabel(new QLabel(tr("Нет"), this))
    , elapsedTimeLabel(new QLabel(tr("Прошло времени:"), this))
    , elapsedTimeValueLabel(new QLabel("00:00:00", this))
    , progressBar(new QProgressBar(this))
    , tabWidget(new QTabWidget(this))
    , updateTimer(new QTimer(this))
{
    setupUi();
    setupConnections();
    updateTaskComboBox();
    updateButtonStates();
    
    // Запуск таймера обновления интерфейса
    updateTimer->start(1000); // Обновление каждую секунду
}

TimeTrackerWidget::~TimeTrackerWidget()
{
    updateTimer->stop();
}

void TimeTrackerWidget::setupUi()
{
    // Создание компоновки
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Группа выбора задачи
    QGroupBox* taskSelectionGroup = new QGroupBox(tr("Выберите задачу:"), this);
    QHBoxLayout* taskSelectionLayout = new QHBoxLayout(taskSelectionGroup);
    taskSelectionLayout->addWidget(taskComboBox);
    taskSelectionLayout->addWidget(startButton);
    taskSelectionLayout->addWidget(pauseButton);
    taskSelectionLayout->addWidget(stopButton);
    
    // Группа текущего статуса
    QGroupBox* statusGroup = new QGroupBox(tr("Статус отслеживания"), this);
    QGridLayout* statusLayout = new QGridLayout(statusGroup);
    statusLayout->addWidget(currentTaskLabel, 0, 0);
    statusLayout->addWidget(currentTaskValueLabel, 0, 1);
    statusLayout->addWidget(elapsedTimeLabel, 1, 0);
    statusLayout->addWidget(elapsedTimeValueLabel, 1, 1);
    statusLayout->addWidget(new QLabel(tr("Сессия прогресс:"), this), 2, 0);
    statusLayout->addWidget(progressBar, 2, 1);
    
    // Создание вкладок для графиков
    tabWidget->addTab(createDistributionTab(), tr("Распределение времени"));
    tabWidget->addTab(createHistoryTab(), tr("История"));
    
    // Добавление всех элементов в основную компоновку
    mainLayout->addWidget(taskSelectionGroup);
    mainLayout->addWidget(statusGroup);
    mainLayout->addWidget(tabWidget, 1);
    
    setLayout(mainLayout);
}

QWidget* TimeTrackerWidget::createDistributionTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Создание графика распределения времени
    QChart* chart = new QChart();
    chart->setTitle(tr("Распределение времени по задачам"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    // Создание серии данных
    QPieSeries* series = new QPieSeries();
    
    // Добавление данных (будет обновляться позже)
    series->append(tr("Нет данных"), 1);
    
    chart->addSeries(series);
    chart->legend()->setAlignment(Qt::AlignRight);
    
    // Создание представления графика
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    layout->addWidget(chartView);
    
    return tab;
}

QWidget* TimeTrackerWidget::createHistoryTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Создание графика истории
    QChart* chart = new QChart();
    chart->setTitle(tr("История отслеживания времени"));
    chart->setAnimationOptions(QChart::SeriesAnimations);
    
    // Создание серии данных
    QBarSeries* series = new QBarSeries();
    QBarSet* set = new QBarSet(tr("Время (часы)"));
    
    // Добавление данных (будет обновляться позже)
    *set << 0;
    
    series->append(set);
    chart->addSeries(series);
    
    // Создание осей
    QStringList categories;
    categories << tr("Нет данных");
    
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
    
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
    
    // Создание представления графика
    QChartView* chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    
    layout->addWidget(chartView);
    
    return tab;
}

void TimeTrackerWidget::setupConnections()
{
    // Соединение сигналов и слотов
    connect(startButton, &QPushButton::clicked, this, &TimeTrackerWidget::startTimer);
    connect(pauseButton, &QPushButton::clicked, this, &TimeTrackerWidget::pauseTimer);
    connect(stopButton, &QPushButton::clicked, this, &TimeTrackerWidget::stopTimer);
    
    connect(updateTimer, &QTimer::timeout, this, &TimeTrackerWidget::updateTimerDisplay);
    
    connect(taskComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TimeTrackerWidget::updateButtonStates);
}

void TimeTrackerWidget::updateTaskComboBox()
{
    // Сохранение текущего выбранного ID
    int currentTaskId = -1;
    if (taskComboBox->currentIndex() != -1) {
        currentTaskId = taskComboBox->currentData().toInt();
    }
    
    // Очистка и заполнение комбобокса
    taskComboBox->clear();
    
    std::vector<Task> tasks = taskManager->getPendingTasks();
    for (const auto& task : tasks) {
        taskComboBox->addItem(QString::fromStdString(task.getTitle()), task.getId());
    }
    
    // Восстановление выбранного элемента, если возможно
    if (currentTaskId != -1) {
        int index = taskComboBox->findData(currentTaskId);
        if (index != -1) {
            taskComboBox->setCurrentIndex(index);
        }
    }
    
    // Обновление состояния кнопок
    updateButtonStates();
}

void TimeTrackerWidget::updateButtonStates()
{
    bool hasTask = taskComboBox->count() > 0;
    bool isRunning = timeTracker->getIsRunning();
    bool isPaused = timeTracker->getIsPaused();
    
    startButton->setEnabled(hasTask && (!isRunning || isPaused));
    pauseButton->setEnabled(isRunning && !isPaused);
    stopButton->setEnabled(isRunning);
    
    taskComboBox->setEnabled(!isRunning);
}

void TimeTrackerWidget::updateTimerDisplay()
{
    if (timeTracker->getIsRunning()) {
        // Обновление отображения времени
        std::string elapsedTime = timeTracker->getElapsedTimeString();
        elapsedTimeValueLabel->setText(QString::fromStdString(elapsedTime));
        
        // Обновление прогресс-бара
        Task* currentTask = timeTracker->getCurrentTask();
        if (currentTask && currentTask->hasDeadline()) {
            QDateTime startTime = QDateTime::fromString(QString::fromStdString(timeTracker->getStartTimeString()), "yyyy-MM-dd hh:mm:ss");
            QDateTime deadline = QDateTime::fromString(QString::fromStdString(currentTask->getDeadline()), "yyyy-MM-dd hh:mm");
            
            if (startTime.isValid() && deadline.isValid()) {
                qint64 totalSeconds = startTime.secsTo(deadline);
                qint64 elapsedSeconds = startTime.secsTo(QDateTime::currentDateTime());
                
                if (totalSeconds > 0) {
                    int progress = (elapsedSeconds * 100) / totalSeconds;
                    progressBar->setValue(qMin(progress, 100));
                } else {
                    progressBar->setValue(100);
                }
            }
        } else {
            progressBar->setValue(0);
        }
    }
    
    // Обновление графиков
    updateCharts();
}

void TimeTrackerWidget::updateCharts()
{
    // Обновление графика распределения времени
    QChart* distributionChart = qobject_cast<QChartView*>(tabWidget->widget(0)->layout()->itemAt(0)->widget())->chart();
    QPieSeries* pieSeries = new QPieSeries();
    
    std::vector<Task> tasks = taskManager->getAllTasks();
    bool hasData = false;
    
    for (const auto& task : tasks) {
        int timeSpent = task.getTimeSpent();
        if (timeSpent > 0) {
            pieSeries->append(QString::fromStdString(task.getTitle()), timeSpent);
            hasData = true;
        }
    }
    
    if (!hasData) {
        pieSeries->append(tr("Другое"), 1);
    }
    
    // Обновление графика
    distributionChart->removeAllSeries();
    distributionChart->addSeries(pieSeries);
    pieSeries->setLabelsVisible(true);
    pieSeries->setLabelsPosition(QPieSlice::LabelOutside);
    
    // Обновление графика истории
    QChart* historyChart = qobject_cast<QChartView*>(tabWidget->widget(1)->layout()->itemAt(0)->widget())->chart();
    QBarSeries* barSeries = new QBarSeries();
    QBarSet* barSet = new QBarSet(tr("Время (часы)"));
    
    // Получение данных за последние 7 дней
    QStringList categories;
    bool historyHasData = false;
    
    for (int i = 6; i >= 0; --i) {
        QDate date = QDate::currentDate().addDays(-i);
        categories << date.toString("dd.MM");
        
        // Суммирование времени за день
        double totalHours = 0;
        for (const auto& task : tasks) {
            // В реальном приложении здесь должна быть логика получения времени за конкретный день
            // Для примера просто добавим случайное значение
            if (i == 0) {
                totalHours += task.getTimeSpent() / 3600.0;
                historyHasData = true;
            }
        }
        
        *barSet << totalHours;
    }
    
    if (!historyHasData) {
        categories.clear();
        barSet->remove(0, barSet->count());
        
        for (int i = 6; i >= 0; --i) {
            QDate date = QDate::currentDate().addDays(-i);
            categories << date.toString("dd.MM");
            *barSet << 0;
        }
    }
    
    barSeries->append(barSet);
    
    // Обновление графика
    historyChart->removeAllSeries();
    historyChart->addSeries(barSeries);
    
    // Обновление осей
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    historyChart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);
    
    QValueAxis* axisY = new QValueAxis();
    double maxValue = 0;
    for (int i = 0; i < barSet->count(); ++i) {
        maxValue = qMax(maxValue, barSet->at(i));
    }
    axisY->setRange(0, qMax(1.0, maxValue * 1.1));
    historyChart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);
}

void TimeTrackerWidget::startTimer()
{
    if (taskComboBox->count() == 0) {
        QMessageBox::warning(this, tr("Нет задачи"), tr("Пожалуйста, выберите задачу для отслеживания"));
        return;
    }
    
    int taskId = taskComboBox->currentData().toInt();
    Task* task = taskManager->getTaskById(taskId);
    
    if (!task) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Задача не найдена"));
        return;
    }
    
    if (timeTracker->getIsRunning() && !timeTracker->getIsPaused()) {
        QMessageBox::warning(this, tr("Таймер уже запущен"), tr("Таймер уже запущен для задачи: %1").arg(QString::fromStdString(timeTracker->getCurrentTask()->getTitle())));
        return;
    }
    
    if (timeTracker->getIsPaused()) {
        timeTracker->resumeTracking();
    } else {
        timeTracker->startTracking(task);
    }
    
    // Обновление интерфейса
    currentTaskValueLabel->setText(QString::fromStdString(task->getTitle()));
    updateButtonStates();
    progressBar->setValue(0);
}

void TimeTrackerWidget::pauseTimer()
{
    if (!timeTracker->getIsRunning() || timeTracker->getIsPaused()) {
        QMessageBox::warning(this, tr("Таймер не запущен"), tr("Нет активного таймера для приостановки или остановки"));
        return;
    }
    
    timeTracker->pauseTracking();
    updateButtonStates();
}

void TimeTrackerWidget::stopTimer()
{
    if (!timeTracker->getIsRunning()) {
        QMessageBox::warning(this, tr("Таймер не запущен"), tr("Нет активного таймера для приостановки или остановки"));
        return;
    }
    
    Task* task = timeTracker->getCurrentTask();
    QString taskTitle = task ? QString::fromStdString(task->getTitle()) : tr("неизвестная задача");
    
    timeTracker->stopTracking();
    
    // Обновление интерфейса
    currentTaskValueLabel->setText(tr("Нет"));
    elapsedTimeValueLabel->setText("00:00:00");
    progressBar->setValue(0);
    updateButtonStates();
    updateTaskComboBox();
    
    QMessageBox::information(this, tr("Отслеживание остановлено"), tr("Отслеживание времени для задачи &quot;%1&quot; остановлено.").arg(taskTitle));
}