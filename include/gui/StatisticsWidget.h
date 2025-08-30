#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QLabel>
#include <QChart>
#include <QChartView>
#include <QBarSeries>
#include <QPieSeries>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QBarCategoryAxis>
#include <QTableView>
#include <QStandardItemModel>
#include <QPushButton>
#include <QComboBox>
#include <QDateEdit>

#include "../core/TaskManager.h"

QT_CHARTS_USE_NAMESPACE

class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(TaskManager *taskManager, QWidget *parent = nullptr);
    ~StatisticsWidget();

    // Публичные методы для обновления статистики
    void updateStatistics();
    void exportStatistics();

signals:
    // Сигналы для уведомления об изменениях
    void statisticsUpdated();

private slots:
    // Слоты для обработки действий пользователя
    void onTabChanged(int index);
    void onPeriodChanged(int index);
    void onExportButtonClicked();
    void onRefreshButtonClicked();
    void onStartDateChanged(const QDate &date);
    void onEndDateChanged(const QDate &date);

private:
    // Настройка интерфейса
    void setupUi();
    void setupConnections();
    
    // Методы для создания различных графиков
    void createTaskCompletionChart();
    void createTimeDistributionChart();
    void createProductivityTrendChart();
    void createPriorityDistributionChart();
    
    // Методы для обновления графиков
    void updateTaskCompletionChart();
    void updateTimeDistributionChart();
    void updateProductivityTrendChart();
    void updatePriorityDistributionChart();
    
    // Методы для обновления таблиц
    void updateTaskStatisticsTable();
    void updateTimeStatisticsTable();
    void updatePomodoroStatisticsTable();
    
    // Вспомогательные методы
    void calculateStatistics();
    QStringList getLastNDays(int n);
    QVector<double> getCompletionRateByDay(int days);
    QMap<QString, double> getTimeSpentByTask();
    QMap<int, int> getTaskCountByPriority();
    
    // Компоненты интерфейса
    QTabWidget *tabWidget;
    
    // Вкладка "Обзор"
    QWidget *overviewTab;
    QLabel *totalTasksLabel;
    QLabel *completedTasksLabel;
    QLabel *pendingTasksLabel;
    QLabel *completionRateLabel;
    QLabel *totalTimeSpentLabel;
    QLabel *averageTimePerTaskLabel;
    QChartView *taskCompletionChartView;
    QChartView *timeDistributionChartView;
    
    // Вкладка "Задачи"
    QWidget *tasksTab;
    QTableView *taskStatisticsTableView;
    QStandardItemModel *taskStatisticsModel;
    QChartView *priorityDistributionChartView;
    
    // Вкладка "Время"
    QWidget *timeTab;
    QTableView *timeStatisticsTableView;
    QStandardItemModel *timeStatisticsModel;
    QChartView *productivityTrendChartView;
    
    // Вкладка "Помодоро"
    QWidget *pomodoroTab;
    QTableView *pomodoroStatisticsTableView;
    QStandardItemModel *pomodoroStatisticsModel;
    QLabel *totalPomodoroSessionsLabel;
    QLabel *totalWorkTimeLabel;
    QLabel *totalBreakTimeLabel;
    
    // Элементы управления периодом
    QComboBox *periodComboBox;
    QDateEdit *startDateEdit;
    QDateEdit *endDateEdit;
    QPushButton *refreshButton;
    QPushButton *exportButton;
    
    // Графики
    QChart *taskCompletionChart;
    QChart *timeDistributionChart;
    QChart *productivityTrendChart;
    QChart *priorityDistributionChart;
    
    // Ссылка на менеджер задач
    TaskManager *taskManager;
    
    // Данные статистики
    int totalTasks;
    int completedTasks;
    int pendingTasks;
    double completionRate;
    std::time_t totalTimeSpent;
    double averageTimePerTask;
    int totalPomodoroSessions;
    std::time_t totalWorkTime;
    std::time_t totalBreakTime;
};

#endif // STATISTICSWIDGET_H