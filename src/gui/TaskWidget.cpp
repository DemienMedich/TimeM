#include "gui/TaskWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTime>
#include <QMenu>
#include <QAction>

TaskWidget::TaskWidget(TaskManager* taskManager, QWidget* parent)
    : QWidget(parent)
    , taskManager(taskManager)
    , taskTable(new QTableWidget(this))
    , addButton(new QPushButton(tr("Добавить задачу"), this))
    , editButton(new QPushButton(tr("Редактировать"), this))
    , deleteButton(new QPushButton(tr("Удалить"), this))
    , completeButton(new QPushButton(tr("Завершить"), this))
    , filterComboStatus(new QComboBox(this))
    , filterComboPriority(new QComboBox(this))
{
    setupUi();
    setupConnections();
    updateTaskList();
}

void TaskWidget::setupUi()
{
    // Настройка таблицы задач
    taskTable->setColumnCount(6);
    taskTable->setHorizontalHeaderLabels({
        tr("ID"), tr("Название"), tr("Приоритет"), tr("Срок"), tr("Статус"), tr("Затраченное время")
    });
    
    taskTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    taskTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    taskTable->setSelectionMode(QAbstractItemView::SingleSelection);
    taskTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    taskTable->setAlternatingRowColors(true);
    taskTable->setSortingEnabled(true);
    
    // Настройка фильтров
    QLabel* filterLabel = new QLabel(tr("Фильтр:"), this);
    
    filterComboStatus->addItem(tr("Все"), -1);
    filterComboStatus->addItem(tr("Ожидающие"), 0);
    filterComboStatus->addItem(tr("Завершенные"), 1);
    
    filterComboPriority->addItem(tr("Любой приоритет"), -1);
    filterComboPriority->addItem(tr("Срочно и Важно"), 1);
    filterComboPriority->addItem(tr("Важно, Не срочно"), 2);
    filterComboPriority->addItem(tr("Срочно, Не важно"), 3);
    filterComboPriority->addItem(tr("Не срочно и Не важно"), 4);
    
    // Создание компоновки
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    QHBoxLayout* filterLayout = new QHBoxLayout();
    filterLayout->addWidget(filterLabel);
    filterLayout->addWidget(filterComboStatus);
    filterLayout->addWidget(filterComboPriority);
    filterLayout->addStretch();
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(editButton);
    buttonLayout->addWidget(deleteButton);
    buttonLayout->addWidget(completeButton);
    
    mainLayout->addLayout(filterLayout);
    mainLayout->addWidget(taskTable);
    mainLayout->addLayout(buttonLayout);
    
    setLayout(mainLayout);
}

void TaskWidget::setupConnections()
{
    // Соединение сигналов и слотов
    connect(addButton, &QPushButton::clicked, this, &TaskWidget::createNewTask);
    connect(editButton, &QPushButton::clicked, this, &TaskWidget::editSelectedTask);
    connect(deleteButton, &QPushButton::clicked, this, &TaskWidget::deleteSelectedTask);
    connect(completeButton, &QPushButton::clicked, this, &TaskWidget::completeSelectedTask);
    
    connect(filterComboStatus, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TaskWidget::updateTaskList);
    connect(filterComboPriority, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &TaskWidget::updateTaskList);
    
    connect(taskTable, &QTableWidget::doubleClicked, this, &TaskWidget::editSelectedTask);
    
    // Контекстное меню
    taskTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(taskTable, &QTableWidget::customContextMenuRequested, this, &TaskWidget::showContextMenu);
}

void TaskWidget::updateTaskList()
{
    taskTable->clearContents();
    taskTable->setRowCount(0);
    
    // Получение фильтров
    int statusFilter = filterComboStatus->currentData().toInt();
    int priorityFilter = filterComboPriority->currentData().toInt();
    
    // Получение списка задач
    std::vector<Task> tasks = taskManager->getAllTasks();
    
    // Фильтрация задач
    std::vector<Task> filteredTasks;
    for (const auto& task : tasks) {
        bool statusMatch = statusFilter == -1 || (statusFilter == 0 && !task.getIsCompleted()) || (statusFilter == 1 && task.getIsCompleted());
        bool priorityMatch = priorityFilter == -1 || task.getPriority() == priorityFilter;
        
        if (statusMatch && priorityMatch) {
            filteredTasks.push_back(task);
        }
    }
    
    // Заполнение таблицы
    taskTable->setRowCount(filteredTasks.size());
    
    for (size_t i = 0; i < filteredTasks.size(); ++i) {
        const Task& task = filteredTasks[i];
        
        // ID
        QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(task.getId()));
        taskTable->setItem(i, 0, idItem);
        
        // Название
        QTableWidgetItem* titleItem = new QTableWidgetItem(QString::fromStdString(task.getTitle()));
        taskTable->setItem(i, 1, titleItem);
        
        // Приоритет
        QString priorityText;
        switch (task.getPriority()) {
            case 1: priorityText = tr("Срочно и Важно"); break;
            case 2: priorityText = tr("Важно, Не срочно"); break;
            case 3: priorityText = tr("Срочно, Не важно"); break;
            case 4: priorityText = tr("Не срочно и Не важно"); break;
            default: priorityText = tr("Неизвестно");
        }
        QTableWidgetItem* priorityItem = new QTableWidgetItem(priorityText);
        taskTable->setItem(i, 2, priorityItem);
        
        // Срок
        QString deadlineText;
        if (task.hasDeadline()) {
            QDateTime deadline = QDateTime::fromString(QString::fromStdString(task.getDeadline()), "yyyy-MM-dd hh:mm");
            deadlineText = deadline.toString("dd.MM.yyyy hh:mm");
        } else {
            deadlineText = tr("Нет срока");
        }
        QTableWidgetItem* deadlineItem = new QTableWidgetItem(deadlineText);
        taskTable->setItem(i, 3, deadlineItem);
        
        // Статус
        QTableWidgetItem* statusItem = new QTableWidgetItem(task.getIsCompleted() ? tr("Выполнено") : tr("Ожидает"));
        taskTable->setItem(i, 4, statusItem);
        
        // Затраченное время
        QTableWidgetItem* timeSpentItem = new QTableWidgetItem(QString::fromStdString(task.getTimeSpentString()));
        taskTable->setItem(i, 5, timeSpentItem);
        
        // Установка цвета фона в зависимости от приоритета
        QColor bgColor;
        if (task.getIsCompleted()) {
            bgColor = QColor(200, 255, 200); // Светло-зеленый для выполненных задач
        } else {
            switch (task.getPriority()) {
                case 1: bgColor = QColor(255, 200, 200); break; // Светло-красный для срочных и важных
                case 2: bgColor = QColor(255, 255, 200); break; // Светло-желтый для важных, но не срочных
                case 3: bgColor = QColor(255, 230, 200); break; // Светло-оранжевый для срочных, но не важных
                case 4: bgColor = QColor(230, 230, 230); break; // Светло-серый для не срочных и не важных
                default: bgColor = QColor(255, 255, 255);
            }
        }
        
        for (int j = 0; j < taskTable->columnCount(); ++j) {
            if (taskTable->item(i, j)) {
                taskTable->item(i, j)->setBackground(bgColor);
            }
        }
    }
    
    // Сортировка по приоритету
    taskTable->sortItems(2);
    
    // Обновление состояния кнопок
    updateButtonStates();
    
    // Отправка сигнала об изменении списка задач
    emit taskListChanged();
}

void TaskWidget::updateButtonStates()
{
    bool hasSelection = taskTable->selectionModel()->hasSelection();
    
    editButton->setEnabled(hasSelection);
    deleteButton->setEnabled(hasSelection);
    completeButton->setEnabled(hasSelection);
    
    if (hasSelection) {
        int row = taskTable->selectionModel()->selectedRows().first().row();
        bool isCompleted = taskTable->item(row, 4)->text() == tr("Выполнено");
        completeButton->setEnabled(!isCompleted);
    }
}

void TaskWidget::createNewTask()
{
    // Создание новой задачи
    bool ok;
    QString title = QInputDialog::getText(this, tr("Новая задача"), tr("Название задачи:"), QLineEdit::Normal, "", &ok);
    
    if (ok && !title.isEmpty()) {
        Task newTask;
        newTask.setTitle(title.toStdString());
        
        // Запрос описания
        QString description = QInputDialog::getMultiLineText(this, tr("Новая задача"), tr("Описание задачи (опционально):"), "", &ok);
        if (ok) {
            newTask.setDescription(description.toStdString());
        }
        
        // Запрос приоритета
        QStringList priorities = {
            tr("Срочно и Важно"),
            tr("Важно, Не срочно"),
            tr("Срочно, Не важно"),
            tr("Не срочно и Не важно")
        };
        
        QString priority = QInputDialog::getItem(this, tr("Новая задача"), tr("Приоритет:"), priorities, 0, false, &ok);
        if (ok) {
            int priorityValue = priorities.indexOf(priority) + 1;
            newTask.setPriority(priorityValue);
        }
        
        // Запрос срока выполнения
        QStringList options = {tr("Нет срока"), tr("Установить срок")};
        QString deadlineOption = QInputDialog::getItem(this, tr("Новая задача"), tr("Срок выполнения:"), options, 0, false, &ok);
        
        if (ok && deadlineOption == tr("Установить срок")) {
            QString deadlineStr = QInputDialog::getText(this, tr("Новая задача"), tr("Введите срок (ГГГГ-ММ-ДД ЧЧ:ММ):"), QLineEdit::Normal, QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm"), &ok);
            
            if (ok && !deadlineStr.isEmpty()) {
                QDateTime deadline = QDateTime::fromString(deadlineStr, "yyyy-MM-dd hh:mm");
                if (deadline.isValid()) {
                    newTask.setDeadline(deadlineStr.toStdString());
                } else {
                    QMessageBox::warning(this, tr("Ошибка"), tr("Неверный формат даты. Используется отсутствие срока."));
                }
            }
        }
        
        // Добавление задачи
        taskManager->addTask(newTask);
        updateTaskList();
        
        QMessageBox::information(this, tr("Успешно"), tr("Задача добавлена успешно!"));
    }
}

void TaskWidget::editSelectedTask()
{
    if (!taskTable->selectionModel()->hasSelection()) {
        return;
    }
    
    int row = taskTable->selectionModel()->selectedRows().first().row();
    int taskId = taskTable->item(row, 0)->text().toInt();
    
    Task* task = taskManager->getTaskById(taskId);
    if (!task) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Задача не найдена."));
        return;
    }
    
    // Редактирование задачи
    bool ok;
    QString title = QInputDialog::getText(this, tr("Редактирование задачи"), tr("Название задачи:"), QLineEdit::Normal, QString::fromStdString(task->getTitle()), &ok);
    
    if (ok && !title.isEmpty()) {
        task->setTitle(title.toStdString());
        
        // Редактирование описания
        QString description = QInputDialog::getMultiLineText(this, tr("Редактирование задачи"), tr("Описание задачи:"), QString::fromStdString(task->getDescription()), &ok);
        if (ok) {
            task->setDescription(description.toStdString());
        }
        
        // Редактирование приоритета
        QStringList priorities = {
            tr("Срочно и Важно"),
            tr("Важно, Не срочно"),
            tr("Срочно, Не важно"),
            tr("Не срочно и Не важно")
        };
        
        int currentPriority = task->getPriority() - 1;
        if (currentPriority < 0 || currentPriority >= priorities.size()) {
            currentPriority = 0;
        }
        
        QString priority = QInputDialog::getItem(this, tr("Редактирование задачи"), tr("Приоритет:"), priorities, currentPriority, false, &ok);
        if (ok) {
            int priorityValue = priorities.indexOf(priority) + 1;
            task->setPriority(priorityValue);
        }
        
        // Редактирование срока выполнения
        QStringList options = {tr("Нет срока"), tr("Установить срок")};
        QString deadlineOption = QInputDialog::getItem(this, tr("Редактирование задачи"), tr("Срок выполнения:"), options, task->hasDeadline() ? 1 : 0, false, &ok);
        
        if (ok) {
            if (deadlineOption == tr("Нет срока")) {
                task->clearDeadline();
            } else {
                QString defaultDeadline = task->hasDeadline() ? 
                    QString::fromStdString(task->getDeadline()) : 
                    QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm");
                
                QString deadlineStr = QInputDialog::getText(this, tr("Редактирование задачи"), tr("Введите срок (ГГГГ-ММ-ДД ЧЧ:ММ):"), QLineEdit::Normal, defaultDeadline, &ok);
                
                if (ok && !deadlineStr.isEmpty()) {
                    QDateTime deadline = QDateTime::fromString(deadlineStr, "yyyy-MM-dd hh:mm");
                    if (deadline.isValid()) {
                        task->setDeadline(deadlineStr.toStdString());
                    } else {
                        QMessageBox::warning(this, tr("Ошибка"), tr("Неверный формат даты. Используется отсутствие срока."));
                        task->clearDeadline();
                    }
                }
            }
        }
        
        updateTaskList();
        QMessageBox::information(this, tr("Успешно"), tr("Задача обновлена успешно!"));
    }
}

void TaskWidget::deleteSelectedTask()
{
    if (!taskTable->selectionModel()->hasSelection()) {
        return;
    }
    
    int row = taskTable->selectionModel()->selectedRows().first().row();
    int taskId = taskTable->item(row, 0)->text().toInt();
    QString taskTitle = taskTable->item(row, 1)->text();
    
    // Подтверждение удаления
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Подтверждение удаления"),
                                                             tr("Вы уверены, что хотите удалить задачу &quot;%1&quot;?").arg(taskTitle),
                                                             QMessageBox::Yes | QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        taskManager->deleteTask(taskId);
        updateTaskList();
        QMessageBox::information(this, tr("Успешно"), tr("Задача удалена успешно!"));
    }
}

void TaskWidget::completeSelectedTask()
{
    if (!taskTable->selectionModel()->hasSelection()) {
        return;
    }
    
    int row = taskTable->selectionModel()->selectedRows().first().row();
    int taskId = taskTable->item(row, 0)->text().toInt();
    
    Task* task = taskManager->getTaskById(taskId);
    if (!task) {
        QMessageBox::warning(this, tr("Ошибка"), tr("Задача не найдена."));
        return;
    }
    
    task->setIsCompleted(true);
    updateTaskList();
    QMessageBox::information(this, tr("Успешно"), tr("Задача отмечена как выполненная!"));
}

void TaskWidget::showContextMenu(const QPoint& pos)
{
    QTableWidgetItem* item = taskTable->itemAt(pos);
    if (!item) {
        return;
    }
    
    QMenu contextMenu(this);
    
    QAction* editAction = new QAction(tr("Редактировать"), this);
    connect(editAction, &QAction::triggered, this, &TaskWidget::editSelectedTask);
    contextMenu.addAction(editAction);
    
    QAction* deleteAction = new QAction(tr("Удалить"), this);
    connect(deleteAction, &QAction::triggered, this, &TaskWidget::deleteSelectedTask);
    contextMenu.addAction(deleteAction);
    
    int row = item->row();
    bool isCompleted = taskTable->item(row, 4)->text() == tr("Выполнено");
    
    if (!isCompleted) {
        QAction* completeAction = new QAction(tr("Завершить"), this);
        connect(completeAction, &QAction::triggered, this, &TaskWidget::completeSelectedTask);
        contextMenu.addAction(completeAction);
    }
    
    contextMenu.exec(taskTable->viewport()->mapToGlobal(pos));
}