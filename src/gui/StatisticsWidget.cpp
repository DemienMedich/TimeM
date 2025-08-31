#include "gui/StatisticsWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QChart>
#include <QPieSeries>
#include <QChartView>
#include <QBarSeries>
#include <QBarSet>
#include <QBarCategoryAxis>
#include <QValueAxis>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QFile>
#include <QTextStream>

using namespace QtCharts;

StatisticsWidget::StatisticsWidget(TaskManager* taskManager, QWidget* parent)
    : QWidget(parent)
    , taskManager(taskManager)
    , tabWidget(new QTabWidget(this))
    , periodComboBox(new QComboBox(this))
    , fromDateEdit(new QDateEdit(this))
    , toDateEdit(new QDateEdit(this))
    , refreshButton(new QPushButton(tr("Обновить"), this))
    , exportButton(new QPushButton(tr("Экспорт"), this))
{
    setupUi();
    setupConnections();
    updateStatistics();
}

void StatisticsWidget::setupUi()
{
    // Настройка компоновки
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Группа фильтров
    QGroupBox* filterGroup = new QGroupBox(tr("Период:"), this);
    QHBoxLayout* filterLayout = new QHBoxLayout(filterGroup);
    
    periodComboBox->addItem(tr("Сегодня"), "today");
    periodComboBox->addItem(tr("Эта неделя"), "week");
    periodComboBox->addItem(tr("Этот месяц"), "month");
    periodComboBox->addItem(tr("Другой"), "custom");
    
    QLabel* fromLabel = new QLabel(tr("С:"), this);
    QLabel* toLabel = new QLabel(tr("По:"), this);
    
    fromDateEdit->setCalendarPopup(true);
    fromDateEdit->setDate(QDate::currentDate().addDays(-7));
    fromDateEdit->setEnabled(false);
    
    toDateEdit->setCalendarPopup(true);
    toDateEdit->setDate(QDate::currentDate());
    toDateEdit->setEnabled(false);
    
    filterLayout->addWidget(periodComboBox);
    filterLayout->addWidget(fromLabel);
    filterLayout->addWidget(fromDateEdit);
    filterLayout->addWidget(toLabel);
    filterLayout->addWidget(toDateEdit);
    filterLayout->addWidget(refreshButton);
    filterLayout->addWidget(exportButton);
    
    // Создание вкладок
    tabWidget->addTab(createOverviewTab(), tr("Обзор"));
    tabWidget->addTab(createTasksTab(), tr("Задачи"));
    tabWidget->addTab(createTimeTab(), tr("Время"));
    tabWidget->addTab(createPomodoroTab(), tr("Помодоро"));
    
    // Добавление всех элементов в основную компоновку
    mainLayout->addWidget(filterGroup);
    mainLayout->addWidget(tabWidget, 1);
    
    setLayout(mainLayout);
}

QWidget* StatisticsWidget::createOverviewTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Создание группы общей статистики
    QGroupBox* overviewGroup = new QGroupBox(tr("Общая статистика"), tab);
    QGridLayout* overviewLayout = new QGridLayout(overviewGroup);
    
    // Метки для статистики
    QLabel* totalTasksLabel = new QLabel(tr("Всего задач:"), overviewGroup);
    QLabel* totalTasksValueLabel = new QLabel("0", overviewGroup);
    totalTasksValueLabel->setObjectName("totalTasksValue");
    
    QLabel* completedTasksLabel = new QLabel(tr("Выполненных задач:"), overviewGroup);
    QLabel* completedTasksValueLabel = new QLabel("0", overviewGroup);
    completedTasksValueLabel->setObjectName("completedTasksValue");
    
    QLabel* pendingTasksLabel = new QLabel(tr("Ожидающих задач:"), overviewGroup);
    QLabel* pendingTasksValueLabel = new QLabel("0", overviewGroup);
    pendingTasksValueLabel->setObjectName("pendingTasksValue");
    
    QLabel* completionRateLabel = new QLabel(tr("Коэффициент выполнения:"), overviewGroup);
    QLabel* completionRateValueLabel = new QLabel("0%", overviewGroup);
    completionRateValueLabel->setObjectName("completionRateValue");
    
    QLabel* totalTimeSpentLabel = new QLabel(tr("Всего затрачено времени:"), overviewGroup);
    QLabel* totalTimeSpentValueLabel = new QLabel("00:00:00", overviewGroup);
    totalTimeSpentValueLabel->setObjectName("totalTimeSpentValue");
    
    QLabel* avgTimePerTaskLabel = new QLabel(tr("Среднее время на задачу:"), overviewGroup);
    QLabel* avgTimePerTaskValueLabel = new QLabel("00:00:00", overviewGroup);
    avgTimePerTaskValueLabel->setObjectName("avgTimePerTaskValue");
    
    QLabel* pomodoroSessionsLabel = new QLabel(tr("Всего сессий Помодоро:"), overviewGroup);
    QLabel* pomodoroSessionsValueLabel = new QLabel("0", overviewGroup);
    pomodoroSessionsValueLabel->setObjectName("pomodoroSessionsValue");
    
    // Добавление меток в компоновку
    overviewLayout->addWidget(totalTasksLabel, 0, 0);
    overviewLayout->addWidget(totalTasksValueLabel, 0, 1);
    overviewLayout->addWidget(completedTasksLabel, 1, 0);
    overviewLayout->addWidget(completedTasksValueLabel, 1, 1);
    overviewLayout->addWidget(pendingTasksLabel, 2, 0);
    overviewLayout->addWidget(pendingTasksValueLabel, 2, 1);
    overviewLayout->addWidget(completionRateLabel, 3, 0);
    overviewLayout->addWidget(completionRateValueLabel, 3, 1);
    overviewLayout->addWidget(totalTimeSpentLabel, 4, 0);
    overviewLayout->addWidget(totalTimeSpentValueLabel, 4, 1);
    overviewLayout->addWidget(avgTimePerTaskLabel, 5, 0);
    overviewLayout->addWidget(avgTimePerTaskValueLabel, 5, 1);
    overviewLayout->addWidget(pomodoroSessionsLabel, 6, 0);
    overviewLayout->addWidget(pomodoroSessionsValueLabel, 6, 1);
    
    // Создание графика выполнения задач
    QChart* taskCompletionChart = new QChart();
    taskCompletionChart->setTitle(tr("Выполнение задач"));
    taskCompletionChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QPieSeries* taskCompletionSeries = new QPieSeries();
    taskCompletionSeries->append(tr("Выполнено"), 0);
    taskCompletionSeries->append(tr("Ожидает"), 0);
    
    taskCompletionChart->addSeries(taskCompletionSeries);
    taskCompletionChart->legend()->setAlignment(Qt::AlignRight);
    
    QChartView* taskCompletionChartView = new QChartView(taskCompletionChart);
    taskCompletionChartView->setRenderHint(QPainter::Antialiasing);
    taskCompletionChartView->setObjectName("taskCompletionChart");
    
    // Создание графика распределения времени
    QChart* timeDistributionChart = new QChart();
    timeDistributionChart->setTitle(tr("Распределение времени"));
    timeDistributionChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QPieSeries* timeDistributionSeries = new QPieSeries();
    timeDistributionSeries->append(tr("Нет данных"), 1);
    
    timeDistributionChart->addSeries(timeDistributionSeries);
    timeDistributionChart->legend()->setAlignment(Qt::AlignRight);
    
    QChartView* timeDistributionChartView = new QChartView(timeDistributionChart);
    timeDistributionChartView->setRenderHint(QPainter::Antialiasing);
    timeDistributionChartView->setObjectName("timeDistributionChart");
    
    // Добавление всех элементов в компоновку вкладки
    layout->addWidget(overviewGroup);
    
    QHBoxLayout* chartsLayout = new QHBoxLayout();
    chartsLayout->addWidget(taskCompletionChartView);
    chartsLayout->addWidget(timeDistributionChartView);
    layout->addLayout(chartsLayout);
    
    return tab;
}

QWidget* StatisticsWidget::createTasksTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Создание группы статистики задач
    QGroupBox* taskStatsGroup = new QGroupBox(tr("Статистика задач"), tab);
    QGridLayout* taskStatsLayout = new QGridLayout(taskStatsGroup);
    
    // Метки для статистики
    QLabel* tasksByPriorityLabel = new QLabel(tr("Задачи по приоритету:"), taskStatsGroup);
    taskStatsLayout->addWidget(tasksByPriorityLabel, 0, 0, 1, 2);
    
    QLabel* priority1Label = new QLabel(tr("Срочно и Важно:"), taskStatsGroup);
    QLabel* priority1ValueLabel = new QLabel("0", taskStatsGroup);
    priority1ValueLabel->setObjectName("priority1Value");
    
    QLabel* priority2Label = new QLabel(tr("Важно, Не срочно:"), taskStatsGroup);
    QLabel* priority2ValueLabel = new QLabel("0", taskStatsGroup);
    priority2ValueLabel->setObjectName("priority2Value");
    
    QLabel* priority3Label = new QLabel(tr("Срочно, Не важно:"), taskStatsGroup);
    QLabel* priority3ValueLabel = new QLabel("0", taskStatsGroup);
    priority3ValueLabel->setObjectName("priority3Value");
    
    QLabel* priority4Label = new QLabel(tr("Не срочно и Не важно:"), taskStatsGroup);
    QLabel* priority4ValueLabel = new QLabel("0", taskStatsGroup);
    priority4ValueLabel->setObjectName("priority4Value");
    
    // Добавление меток в компоновку
    taskStatsLayout->addWidget(priority1Label, 1, 0);
    taskStatsLayout->addWidget(priority1ValueLabel, 1, 1);
    taskStatsLayout->addWidget(priority2Label, 2, 0);
    taskStatsLayout->addWidget(priority2ValueLabel, 2, 1);
    taskStatsLayout->addWidget(priority3Label, 3, 0);
    taskStatsLayout->addWidget(priority3ValueLabel, 3, 1);
    taskStatsLayout->addWidget(priority4Label, 4, 0);
    taskStatsLayout->addWidget(priority4ValueLabel, 4, 1);
    
    // Создание графика распределения по приоритетам
    QChart* priorityDistributionChart = new QChart();
    priorityDistributionChart->setTitle(tr("Распределение по приоритетам"));
    priorityDistributionChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QPieSeries* priorityDistributionSeries = new QPieSeries();
    priorityDistributionSeries->append(tr("Срочно и Важно"), 0);
    priorityDistributionSeries->append(tr("Важно, Не срочно"), 0);
    priorityDistributionSeries->append(tr("Срочно, Не важно"), 0);
    priorityDistributionSeries->append(tr("Не срочно и Не важно"), 0);
    
    priorityDistributionChart->addSeries(priorityDistributionSeries);
    priorityDistributionChart->legend()->setAlignment(Qt::AlignRight);
    
    QChartView* priorityDistributionChartView = new QChartView(priorityDistributionChart);
    priorityDistributionChartView->setRenderHint(QPainter::Antialiasing);
    priorityDistributionChartView->setObjectName("priorityDistributionChart");
    
    // Создание графика выполнения задач по дням
    QChart* taskCompletionTrendChart = new QChart();
    taskCompletionTrendChart->setTitle(tr("Тренд выполнения задач"));
    taskCompletionTrendChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries* taskCompletionTrendSeries = new QBarSeries();
    QBarSet* completedTasksSet = new QBarSet(tr("Выполнено"));
    QBarSet* createdTasksSet = new QBarSet(tr("Создано"));
    
    // Добавление данных (будет обновляться позже)
    *completedTasksSet << 0;
    *createdTasksSet << 0;
    
    taskCompletionTrendSeries->append(completedTasksSet);
    taskCompletionTrendSeries->append(createdTasksSet);
    
    taskCompletionTrendChart->addSeries(taskCompletionTrendSeries);
    
    // Создание осей
    QStringList categories;
    categories << tr("Нет данных");
    
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    taskCompletionTrendChart->addAxis(axisX, Qt::AlignBottom);
    taskCompletionTrendSeries->attachAxis(axisX);
    
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1);
    taskCompletionTrendChart->addAxis(axisY, Qt::AlignLeft);
    taskCompletionTrendSeries->attachAxis(axisY);
    
    QChartView* taskCompletionTrendChartView = new QChartView(taskCompletionTrendChart);
    taskCompletionTrendChartView->setRenderHint(QPainter::Antialiasing);
    taskCompletionTrendChartView->setObjectName("taskCompletionTrendChart");
    
    // Добавление всех элементов в компоновку вкладки
    layout->addWidget(taskStatsGroup);
    
    QHBoxLayout* chartsLayout = new QHBoxLayout();
    chartsLayout->addWidget(priorityDistributionChartView);
    chartsLayout->addWidget(taskCompletionTrendChartView);
    layout->addLayout(chartsLayout);
    
    return tab;
}

QWidget* StatisticsWidget::createTimeTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Создание группы статистики времени
    QGroupBox* timeStatsGroup = new QGroupBox(tr("Статистика времени"), tab);
    QGridLayout* timeStatsLayout = new QGridLayout(timeStatsGroup);
    
    // Метки для статистики
    QLabel* totalTimeSpentLabel = new QLabel(tr("Всего затрачено времени:"), timeStatsGroup);
    QLabel* totalTimeSpentValueLabel = new QLabel("00:00:00", timeStatsGroup);
    totalTimeSpentValueLabel->setObjectName("timeTabTotalTimeSpentValue");
    
    QLabel* avgTimePerTaskLabel = new QLabel(tr("Среднее время на задачу:"), timeStatsGroup);
    QLabel* avgTimePerTaskValueLabel = new QLabel("00:00:00", timeStatsGroup);
    avgTimePerTaskValueLabel->setObjectName("timeTabAvgTimePerTaskValue");
    
    QLabel* mostTimeConsumingTaskLabel = new QLabel(tr("Самая времязатратная задача:"), timeStatsGroup);
    QLabel* mostTimeConsumingTaskValueLabel = new QLabel(tr("Нет данных"), timeStatsGroup);
    mostTimeConsumingTaskValueLabel->setObjectName("mostTimeConsumingTaskValue");
    
    // Добавление меток в компоновку
    timeStatsLayout->addWidget(totalTimeSpentLabel, 0, 0);
    timeStatsLayout->addWidget(totalTimeSpentValueLabel, 0, 1);
    timeStatsLayout->addWidget(avgTimePerTaskLabel, 1, 0);
    timeStatsLayout->addWidget(avgTimePerTaskValueLabel, 1, 1);
    timeStatsLayout->addWidget(mostTimeConsumingTaskLabel, 2, 0);
    timeStatsLayout->addWidget(mostTimeConsumingTaskValueLabel, 2, 1);
    
    // Создание графика продуктивности
    QChart* productivityTrendChart = new QChart();
    productivityTrendChart->setTitle(tr("Тренд продуктивности"));
    productivityTrendChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QLineSeries* productivityTrendSeries = new QLineSeries();
    productivityTrendSeries->setName(tr("Часы работы"));
    
    // Добавление данных (будет обновляться позже)
    productivityTrendSeries->append(0, 0);
    
    productivityTrendChart->addSeries(productivityTrendSeries);
    
    // Создание осей
    QDateTimeAxis* axisX = new QDateTimeAxis();
    axisX->setFormat("dd.MM");
    axisX->setTitleText(tr("Дата"));
    productivityTrendChart->addAxis(axisX, Qt::AlignBottom);
    productivityTrendSeries->attachAxis(axisX);
    
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1);
    axisY->setTitleText(tr("Часы"));
    productivityTrendChart->addAxis(axisY, Qt::AlignLeft);
    productivityTrendSeries->attachAxis(axisY);
    
    QChartView* productivityTrendChartView = new QChartView(productivityTrendChart);
    productivityTrendChartView->setRenderHint(QPainter::Antialiasing);
    productivityTrendChartView->setObjectName("productivityTrendChart");
    
    // Добавление всех элементов в компоновку вкладки
    layout->addWidget(timeStatsGroup);
    layout->addWidget(productivityTrendChartView);
    
    return tab;
}

QWidget* StatisticsWidget::createPomodoroTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Создание группы статистики Помодоро
    QGroupBox* pomodoroStatsGroup = new QGroupBox(tr("Статистика Помодоро"), tab);
    QGridLayout* pomodoroStatsLayout = new QGridLayout(pomodoroStatsGroup);
    
    // Метки для статистики
    QLabel* totalPomodoroSessionsLabel = new QLabel(tr("Всего сессий Помодоро:"), pomodoroStatsGroup);
    QLabel* totalPomodoroSessionsValueLabel = new QLabel("0", pomodoroStatsGroup);
    totalPomodoroSessionsValueLabel->setObjectName("totalPomodoroSessionsValue");
    
    QLabel* totalWorkTimeLabel = new QLabel(tr("Общее рабочее время:"), pomodoroStatsGroup);
    QLabel* totalWorkTimeValueLabel = new QLabel("00:00:00", pomodoroStatsGroup);
    totalWorkTimeValueLabel->setObjectName("totalWorkTimeValue");
    
    QLabel* totalBreakTimeLabel = new QLabel(tr("Общее время перерывов:"), pomodoroStatsGroup);
    QLabel* totalBreakTimeValueLabel = new QLabel("00:00:00", pomodoroStatsGroup);
    totalBreakTimeValueLabel->setObjectName("totalBreakTimeValue");
    
    QLabel* workBreakRatioLabel = new QLabel(tr("Соотношение работа/перерыв:"), pomodoroStatsGroup);
    QLabel* workBreakRatioValueLabel = new QLabel("0:0", pomodoroStatsGroup);
    workBreakRatioValueLabel->setObjectName("workBreakRatioValue");
    
    // Добавление меток в компоновку
    pomodoroStatsLayout->addWidget(totalPomodoroSessionsLabel, 0, 0);
    pomodoroStatsLayout->addWidget(totalPomodoroSessionsValueLabel, 0, 1);
    pomodoroStatsLayout->addWidget(totalWorkTimeLabel, 1, 0);
    pomodoroStatsLayout->addWidget(totalWorkTimeValueLabel, 1, 1);
    pomodoroStatsLayout->addWidget(totalBreakTimeLabel, 2, 0);
    pomodoroStatsLayout->addWidget(totalBreakTimeValueLabel, 2, 1);
    pomodoroStatsLayout->addWidget(workBreakRatioLabel, 3, 0);
    pomodoroStatsLayout->addWidget(workBreakRatioValueLabel, 3, 1);
    
    // Создание графика сессий Помодоро по дням
    QChart* pomodoroSessionsChart = new QChart();
    pomodoroSessionsChart->setTitle(tr("Сессии Помодоро по дням"));
    pomodoroSessionsChart->setAnimationOptions(QChart::SeriesAnimations);
    
    QBarSeries* pomodoroSessionsSeries = new QBarSeries();
    QBarSet* workSessionsSet = new QBarSet(tr("Рабочие сессии"));
    QBarSet* breakSessionsSet = new QBarSet(tr("Перерывы"));
    
    // Добавление данных (будет обновляться позже)
    *workSessionsSet << 0;
    *breakSessionsSet << 0;
    
    pomodoroSessionsSeries->append(workSessionsSet);
    pomodoroSessionsSeries->append(breakSessionsSet);
    
    pomodoroSessionsChart->addSeries(pomodoroSessionsSeries);
    
    // Создание осей
    QStringList categories;
    categories << tr("Нет данных");
    
    QBarCategoryAxis* axisX = new QBarCategoryAxis();
    axisX->append(categories);
    pomodoroSessionsChart->addAxis(axisX, Qt::AlignBottom);
    pomodoroSessionsSeries->attachAxis(axisX);
    
    QValueAxis* axisY = new QValueAxis();
    axisY->setRange(0, 1);
    pomodoroSessionsChart->addAxis(axisY, Qt::AlignLeft);
    pomodoroSessionsSeries->attachAxis(axisY);
    
    QChartView* pomodoroSessionsChartView = new QChartView(pomodoroSessionsChart);
    pomodoroSessionsChartView->setRenderHint(QPainter::Antialiasing);
    pomodoroSessionsChartView->setObjectName("pomodoroSessionsChart");
    
    // Добавление всех элементов в компоновку вкладки
    layout->addWidget(pomodoroStatsGroup);
    layout->addWidget(pomodoroSessionsChartView);
    
    return tab;
}

void StatisticsWidget::setupConnections()
{
    // Соединение сигналов и слотов
    connect(periodComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &StatisticsWidget::onPeriodChanged);
    connect(refreshButton, &QPushButton::clicked, this, &StatisticsWidget::updateStatistics);
    connect(exportButton, &QPushButton::clicked, this, &StatisticsWidget::exportStatistics);
}

void StatisticsWidget::onPeriodChanged(int index)
{
    QString period = periodComboBox->itemData(index).toString();
    
    bool isCustom = (period == "custom");
    fromDateEdit->setEnabled(isCustom);
    toDateEdit->setEnabled(isCustom);
    
    if (!isCustom) {
        QDate fromDate;
        QDate toDate = QDate::currentDate();
        
        if (period == "today") {
            fromDate = toDate;
        } else if (period == "week") {
            fromDate = toDate.addDays(-toDate.dayOfWeek() + 1);
        } else if (period == "month") {
            fromDate = QDate(toDate.year(), toDate.month(), 1);
        }
        
        fromDateEdit->setDate(fromDate);
        toDateEdit->setDate(toDate);
    }
    
    updateStatistics();
}

void StatisticsWidget::updateStatistics()
{
    // Получение статистики задач
    int totalTasks = taskManager->getTotalTaskCount();
    int completedTasks = taskManager->getCompletedTaskCount();
    int pendingTasks = taskManager->getPendingTaskCount();
    
    // Расчет коэффициента выполнения
    double completionRate = totalTasks > 0 ? (double)completedTasks / totalTasks * 100 : 0;
    
    // Получение общего затраченного времени
    int totalTimeSpent = 0;
    std::vector<Task> tasks = taskManager->getAllTasks();
    for (const auto& task : tasks) {
        totalTimeSpent += task.getTimeSpent();
    }
    
    // Расчет среднего времени на задачу
    int avgTimePerTask = totalTasks > 0 ? totalTimeSpent / totalTasks : 0;
    
    // Форматирование времени
    auto formatTime = [](int seconds) -> QString {
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        int secs = seconds % 60;
        return QString("%1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0'));
    };
    
    // Обновление меток на вкладке "Обзор"
    QWidget* overviewTab = tabWidget->widget(0);
    overviewTab->findChild<QLabel*>("totalTasksValue")->setText(QString::number(totalTasks));
    overviewTab->findChild<QLabel*>("completedTasksValue")->setText(QString::number(completedTasks));
    overviewTab->findChild<QLabel*>("pendingTasksValue")->setText(QString::number(pendingTasks));
    overviewTab->findChild<QLabel*>("completionRateValue")->setText(QString("%1%").arg(completionRate, 0, 'f', 1));
    overviewTab->findChild<QLabel*>("totalTimeSpentValue")->setText(formatTime(totalTimeSpent));
    overviewTab->findChild<QLabel*>("avgTimePerTaskValue")->setText(formatTime(avgTimePerTask));
    
    // Обновление графика выполнения задач
    QChartView* taskCompletionChartView = overviewTab->findChild<QChartView*>("taskCompletionChart");
    QChart* taskCompletionChart = taskCompletionChartView->chart();
    QPieSeries* taskCompletionSeries = new QPieSeries();
    
    if (totalTasks > 0) {
        QPieSlice* completedSlice = taskCompletionSeries->append(tr("Выполнено"), completedTasks);
        completedSlice->setBrush(QColor(92, 184, 92)); // Зеленый
        
        QPieSlice* pendingSlice = taskCompletionSeries->append(tr("Ожидает"), pendingTasks);
        pendingSlice->setBrush(QColor(240, 173, 78)); // Оранжевый
    } else {
        taskCompletionSeries->append(tr("Нет данных"), 1);
    }
    
    taskCompletionChart->removeAllSeries();
    taskCompletionChart->addSeries(taskCompletionSeries);
    
    // Обновление графика распределения времени
    QChartView* timeDistributionChartView = overviewTab->findChild<QChartView*>("timeDistributionChart");
    QChart* timeDistributionChart = timeDistributionChartView->chart();
    QPieSeries* timeDistributionSeries = new QPieSeries();
    
    bool hasTimeData = false;
    for (const auto& task : tasks) {
        int timeSpent = task.getTimeSpent();
        if (timeSpent > 0) {
            timeDistributionSeries->append(QString::fromStdString(task.getTitle()), timeSpent);
            hasTimeData = true;
        }
    }
    
    if (!hasTimeData) {
        timeDistributionSeries->append(tr("Нет данных"), 1);
    }
    
    timeDistributionChart->removeAllSeries();
    timeDistributionChart->addSeries(timeDistributionSeries);
    
    // Обновление вкладки "Задачи"
    QWidget* tasksTab = tabWidget->widget(1);
    
    // Подсчет задач по приоритетам
    int priority1Count = 0, priority2Count = 0, priority3Count = 0, priority4Count = 0;
    for (const auto& task : tasks) {
        switch (task.getPriority()) {
            case 1: priority1Count++; break;
            case 2: priority2Count++; break;
            case 3: priority3Count++; break;
            case 4: priority4Count++; break;
        }
    }
    
    tasksTab->findChild<QLabel*>("priority1Value")->setText(QString::number(priority1Count));
    tasksTab->findChild<QLabel*>("priority2Value")->setText(QString::number(priority2Count));
    tasksTab->findChild<QLabel*>("priority3Value")->setText(QString::number(priority3Count));
    tasksTab->findChild<QLabel*>("priority4Value")->setText(QString::number(priority4Count));
    
    // Обновление графика распределения по приоритетам
    QChartView* priorityDistributionChartView = tasksTab->findChild<QChartView*>("priorityDistributionChart");
    QChart* priorityDistributionChart = priorityDistributionChartView->chart();
    QPieSeries* priorityDistributionSeries = new QPieSeries();
    
    if (totalTasks > 0) {
        if (priority1Count > 0) {
            QPieSlice* slice = priorityDistributionSeries->append(tr("Срочно и Важно"), priority1Count);
            slice->setBrush(QColor(217, 83, 79)); // Красный
        }
        
        if (priority2Count > 0) {
            QPieSlice* slice = priorityDistributionSeries->append(tr("Важно, Не срочно"), priority2Count);
            slice->setBrush(QColor(91, 192, 222)); // Синий
        }
        
        if (priority3Count > 0) {
            QPieSlice* slice = priorityDistributionSeries->append(tr("Срочно, Не важно"), priority3Count);
            slice->setBrush(QColor(240, 173, 78)); // Оранжевый
        }
        
        if (priority4Count > 0) {
            QPieSlice* slice = priorityDistributionSeries->append(tr("Не срочно и Не важно"), priority4Count);
            slice->setBrush(QColor(153, 153, 153)); // Серый
        }
    } else {
        priorityDistributionSeries->append(tr("Нет данных"), 1);
    }
    
    priorityDistributionChart->removeAllSeries();
    priorityDistributionChart->addSeries(priorityDistributionSeries);
    
    // Обновление вкладки "Время"
    QWidget* timeTab = tabWidget->widget(2);
    timeTab->findChild<QLabel*>("timeTabTotalTimeSpentValue")->setText(formatTime(totalTimeSpent));
    timeTab->findChild<QLabel*>("timeTabAvgTimePerTaskValue")->setText(formatTime(avgTimePerTask));
    
    // Поиск самой времязатратной задачи
    Task* mostTimeConsumingTask = nullptr;
    int maxTimeSpent = 0;
    for (auto& task : tasks) {
        if (task.getTimeSpent() > maxTimeSpent) {
            maxTimeSpent = task.getTimeSpent();
            mostTimeConsumingTask = &task;
        }
    }
    
    if (mostTimeConsumingTask) {
        QString taskInfo = QString("%1 (%2)")
            .arg(QString::fromStdString(mostTimeConsumingTask->getTitle()))
            .arg(formatTime(mostTimeConsumingTask->getTimeSpent()));
        timeTab->findChild<QLabel*>("mostTimeConsumingTaskValue")->setText(taskInfo);
    } else {
        timeTab->findChild<QLabel*>("mostTimeConsumingTaskValue")->setText(tr("Нет данных"));
    }
}

void StatisticsWidget::exportStatistics()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Экспорт статистики"),
                                                   QDir::homePath() + "/statistics.csv",
                                                   tr("CSV файлы (*.csv);;Все файлы (*)"));
    
    if (fileName.isEmpty()) {
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, tr("Ошибка экспорта"), tr("Не удалось открыть файл для записи."));
        return;
    }
    
    QTextStream out(&file);
    
    // Запись заголовка
    out << tr("Статистика приложения для управления временем") << "\n\n";
    
    // Запись периода
    QDate fromDate = fromDateEdit->date();
    QDate toDate = toDateEdit->date();
    out << tr("Период: %1 - %2").arg(fromDate.toString("dd.MM.yyyy")).arg(toDate.toString("dd.MM.yyyy")) << "\n\n";
    
    // Запись общей статистики
    out << tr("Общая статистика") << "\n";
    out << tr("Всего задач") << "," << taskManager->getTotalTaskCount() << "\n";
    out << tr("Выполненных задач") << "," << taskManager->getCompletedTaskCount() << "\n";
    out << tr("Ожидающих задач") << "," << taskManager->getPendingTaskCount() << "\n";
    
    // Расчет коэффициента выполнения
    int totalTasks = taskManager->getTotalTaskCount();
    int completedTasks = taskManager->getCompletedTaskCount();
    double completionRate = totalTasks > 0 ? (double)completedTasks / totalTasks * 100 : 0;
    out << tr("Коэффициент выполнения") << "," << QString::number(completionRate, 'f', 1) << "%\n\n";
    
    // Запись статистики по задачам
    out << tr("Статистика задач") << "\n";
    out << tr("ID,Название,Приоритет,Срок,Статус,Затраченное время (сек)") << "\n";
    
    std::vector<Task> tasks = taskManager->getAllTasks();
    for (const auto& task : tasks) {
        QString priority;
        switch (task.getPriority()) {
            case 1: priority = tr("Срочно и Важно"); break;
            case 2: priority = tr("Важно, Не срочно"); break;
            case 3: priority = tr("Срочно, Не важно"); break;
            case 4: priority = tr("Не срочно и Не важно"); break;
            default: priority = tr("Неизвестно");
        }
        
        QString deadline = task.hasDeadline() ? QString::fromStdString(task.getDeadline()) : tr("Нет срока");
        QString status = task.getIsCompleted() ? tr("Выполнено") : tr("Ожидает");
        
        out << task.getId() << ","
            << "&quot;" << QString::fromStdString(task.getTitle()) << "&quot;,"
            << "&quot;" << priority << "&quot;,"
            << "&quot;" << deadline << "&quot;,"
            << "&quot;" << status << "&quot;,"
            << task.getTimeSpent() << "\n";
    }
    
    file.close();
    
    QMessageBox::information(this, tr("Экспорт завершен"), tr("Статистика успешно экспортирована в файл:\n%1").arg(fileName));
}