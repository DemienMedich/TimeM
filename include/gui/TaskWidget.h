#ifndef TASKWIDGET_H
#define TASKWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QSortFilterProxyModel>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>

#include "../core/TaskManager.h"

class TaskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TaskWidget(TaskManager *taskManager, QWidget *parent = nullptr);
    ~TaskWidget();

    // Публичные методы для работы с задачами
    void updateTaskList();
    void createNewTask();
    void editSelectedTask();
    void deleteSelectedTask();

signals:
    // Сигналы для уведомления об изменениях
    void taskListChanged();
    void taskSelected(int taskId);

private slots:
    // Слоты для обработки действий пользователя
    void onAddTaskClicked();
    void onEditTaskClicked();
    void onDeleteTaskClicked();
    void onCompleteTaskClicked();
    void onTaskSelectionChanged();
    void onFilterTextChanged(const QString &text);
    void onFilterStatusChanged(int index);
    void onFilterPriorityChanged(int index);
    void onTableContextMenu(const QPoint &pos);
    void onTaskDoubleClicked(const QModelIndex &index);

private:
    // Настройка интерфейса
    void setupUi();
    void setupConnections();
    void setupContextMenu();
    
    // Вспомогательные методы
    void fillTaskTable();
    Task* getSelectedTask();
    int getSelectedTaskId();
    QString getPriorityString(int priority);
    QString getStatusString(bool completed);
    QIcon getPriorityIcon(int priority);
    QIcon getStatusIcon(bool completed);
    
    // Методы для работы с диалогами
    bool showTaskDialog(Task &task, bool isNew = false);
    
    // Компоненты интерфейса
    QTableView *taskTableView;
    QStandardItemModel *taskModel;
    QSortFilterProxyModel *proxyModel;
    
    QLineEdit *filterLineEdit;
    QComboBox *statusFilterComboBox;
    QComboBox *priorityFilterComboBox;
    
    QPushButton *addTaskButton;
    QPushButton *editTaskButton;
    QPushButton *deleteTaskButton;
    QPushButton *completeTaskButton;
    
    QMenu *contextMenu;
    QAction *actionEditTask;
    QAction *actionDeleteTask;
    QAction *actionCompleteTask;
    QAction *actionAddSubtask;
    
    // Ссылка на менеджер задач
    TaskManager *taskManager;
    
    // Константы для колонок таблицы
    enum {
        COLUMN_ID,
        COLUMN_TITLE,
        COLUMN_PRIORITY,
        COLUMN_DEADLINE,
        COLUMN_STATUS,
        COLUMN_TIME_SPENT,
        COLUMN_COUNT
    };
};

#endif // TASKWIDGET_H