#include "gui/MainWindow.h"
#include <QApplication>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QLabel>
#include <QVBoxLayout>
#include <QTranslator>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(nullptr)
    , tabWidget(new QTabWidget(this))
    , mainToolBar(new QToolBar(this))
    , mainStatusBar(new QStatusBar(this))
{
    // Настройка интерфейса
    setupUi();
    
    // Загрузка данных
    loadData();
    
    // Настройка соединений
    setupConnections();
    
    // Обновление статусной строки
    updateStatusBar();
    
    // Установка заголовка окна
    setWindowTitle(tr("Управление временем"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Сохранение данных перед закрытием
    saveData();
    event->accept();
}

void MainWindow::changeEvent(QEvent *event)
{
    if (event->type() == QEvent::LanguageChange) {
        // Обновление текстов при изменении языка
        setWindowTitle(tr("Управление временем"));
        
        // Обновление текстов в меню
        actionNewTask->setText(tr("Новая задача"));
        actionEditTask->setText(tr("Редактировать задачу"));
        actionDeleteTask->setText(tr("Удалить задачу"));
        actionStartTimer->setText(tr("Начать отслеживание"));
        actionPauseTimer->setText(tr("Приостановить отслеживание"));
        actionStopTimer->setText(tr("Остановить отслеживание"));
        actionStartPomodoro->setText(tr("Запустить Помодоро"));
        actionSettings->setText(tr("Настройки"));
        actionAbout->setText(tr("О программе"));
        actionExit->setText(tr("Выход"));
        actionRussian->setText(tr("Русский"));
        actionEnglish->setText(tr("English"));
        actionLightTheme->setText(tr("Светлая тема"));
        actionDarkTheme->setText(tr("Тёмная тема"));
        
        // Обновление заголовков вкладок
        tabWidget->setTabText(0, tr("Задачи"));
        tabWidget->setTabText(1, tr("Отслеживание времени"));
        tabWidget->setTabText(2, tr("Помодоро"));
        tabWidget->setTabText(3, tr("Статистика"));
    }
    
    QMainWindow::changeEvent(event);
}

void MainWindow::setupUi()
{
    // Установка центрального виджета
    setCentralWidget(tabWidget);
    
    // Создание виджетов для вкладок
    taskWidget = new TaskWidget(&taskManager, this);
    timeTrackerWidget = new TimeTrackerWidget(&timeTracker, &taskManager, this);
    pomodoroWidget = new PomodoroWidget(&pomodoroTimer, this);
    statisticsWidget = new StatisticsWidget(&taskManager, this);
    
    // Добавление вкладок
    tabWidget->addTab(taskWidget, tr("Задачи"));
    tabWidget->addTab(timeTrackerWidget, tr("Отслеживание времени"));
    tabWidget->addTab(pomodoroWidget, tr("Помодоро"));
    tabWidget->addTab(statisticsWidget, tr("Статистика"));
    
    // Настройка панели инструментов и меню
    setupToolbar();
    setupMenus();
    setupStatusBar();
    
    // Установка размеров окна
    resize(800, 600);
}

void MainWindow::setupMenus()
{
    // Создание меню "Файл"
    QMenu *fileMenu = menuBar()->addMenu(tr("Файл"));
    
    actionNewTask = new QAction(QIcon(":/icons/add.png"), tr("Новая задача"), this);
    actionNewTask->setShortcut(QKeySequence::New);
    fileMenu->addAction(actionNewTask);
    
    actionEditTask = new QAction(QIcon(":/icons/edit.png"), tr("Редактировать задачу"), this);
    actionEditTask->setShortcut(QKeySequence("Ctrl+E"));
    fileMenu->addAction(actionEditTask);
    
    actionDeleteTask = new QAction(QIcon(":/icons/delete.png"), tr("Удалить задачу"), this);
    actionDeleteTask->setShortcut(QKeySequence::Delete);
    fileMenu->addAction(actionDeleteTask);
    
    fileMenu->addSeparator();
    
    actionExit = new QAction(tr("Выход"), this);
    actionExit->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(actionExit);
    
    // Создание меню "Инструменты"
    QMenu *toolsMenu = menuBar()->addMenu(tr("Инструменты"));
    
    actionStartTimer = new QAction(QIcon(":/icons/start.png"), tr("Начать отслеживание"), this);
    actionStartTimer->setShortcut(QKeySequence("Ctrl+T"));
    toolsMenu->addAction(actionStartTimer);
    
    actionPauseTimer = new QAction(QIcon(":/icons/pause.png"), tr("Приостановить отслеживание"), this);
    actionPauseTimer->setShortcut(QKeySequence("Ctrl+P"));
    toolsMenu->addAction(actionPauseTimer);
    
    actionStopTimer = new QAction(QIcon(":/icons/stop.png"), tr("Остановить отслеживание"), this);
    actionStopTimer->setShortcut(QKeySequence("Ctrl+S"));
    toolsMenu->addAction(actionStopTimer);
    
    toolsMenu->addSeparator();
    
    actionStartPomodoro = new QAction(QIcon(":/icons/pomodoro.png"), tr("Запустить Помодоро"), this);
    actionStartPomodoro->setShortcut(QKeySequence("Ctrl+D"));
    toolsMenu->addAction(actionStartPomodoro);
    
    // Создание меню "Настройки"
    QMenu *settingsMenu = menuBar()->addMenu(tr("Настройки"));
    
    actionSettings = new QAction(QIcon(":/icons/settings.png"), tr("Настройки"), this);
    settingsMenu->addAction(actionSettings);
    
    settingsMenu->addSeparator();
    
    // Подменю "Язык"
    QMenu *languageMenu = settingsMenu->addMenu(tr("Язык"));
    
    actionRussian = new QAction(tr("Русский"), this);
    actionRussian->setCheckable(true);
    languageMenu->addAction(actionRussian);
    
    actionEnglish = new QAction(tr("English"), this);
    actionEnglish->setCheckable(true);
    languageMenu->addAction(actionEnglish);
    
    // Проверка текущего языка
    QString currentLanguage = settings.value("app/language", "en").toString();
    if (currentLanguage == "ru") {
        actionRussian->setChecked(true);
    } else {
        actionEnglish->setChecked(true);
    }
    
    // Подменю "Тема"
    QMenu *themeMenu = settingsMenu->addMenu(tr("Тема"));
    
    actionLightTheme = new QAction(tr("Светлая тема"), this);
    actionLightTheme->setCheckable(true);
    themeMenu->addAction(actionLightTheme);
    
    actionDarkTheme = new QAction(tr("Тёмная тема"), this);
    actionDarkTheme->setCheckable(true);
    themeMenu->addAction(actionDarkTheme);
    
    // Проверка текущей темы
    QString currentTheme = settings.value("app/theme", "light").toString();
    if (currentTheme == "dark") {
        actionDarkTheme->setChecked(true);
    } else {
        actionLightTheme->setChecked(true);
    }
    
    // Создание меню "Справка"
    QMenu *helpMenu = menuBar()->addMenu(tr("Справка"));
    
    actionAbout = new QAction(tr("О программе"), this);
    helpMenu->addAction(actionAbout);
}

void MainWindow::setupToolbar()
{
    // Добавление панели инструментов
    addToolBar(mainToolBar);
    mainToolBar->setMovable(false);
    
    // Добавление действий на панель инструментов
    mainToolBar->addAction(actionNewTask);
    mainToolBar->addAction(actionEditTask);
    mainToolBar->addAction(actionDeleteTask);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionStartTimer);
    mainToolBar->addAction(actionPauseTimer);
    mainToolBar->addAction(actionStopTimer);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionStartPomodoro);
    mainToolBar->addSeparator();
    mainToolBar->addAction(actionSettings);
}

void MainWindow::setupStatusBar()
{
    // Установка статусной строки
    setStatusBar(mainStatusBar);
    
    // Добавление постоянных элементов в статусную строку
    QLabel *taskCountLabel = new QLabel(this);
    mainStatusBar->addPermanentWidget(taskCountLabel);
    
    QLabel *timerStatusLabel = new QLabel(this);
    mainStatusBar->addPermanentWidget(timerStatusLabel);
    
    QLabel *pomodoroStatusLabel = new QLabel(this);
    mainStatusBar->addPermanentWidget(pomodoroStatusLabel);
}

void MainWindow::setupConnections()
{
    // Соединение действий меню с соответствующими слотами
    connect(actionNewTask, &QAction::triggered, this, &MainWindow::onActionNewTask);
    connect(actionEditTask, &QAction::triggered, this, &MainWindow::onActionEditTask);
    connect(actionDeleteTask, &QAction::triggered, this, &MainWindow::onActionDeleteTask);
    connect(actionStartTimer, &QAction::triggered, this, &MainWindow::onActionStartTimer);
    connect(actionPauseTimer, &QAction::triggered, this, &MainWindow::onActionPauseTimer);
    connect(actionStopTimer, &QAction::triggered, this, &MainWindow::onActionStopTimer);
    connect(actionStartPomodoro, &QAction::triggered, this, &MainWindow::onActionStartPomodoro);
    connect(actionSettings, &QAction::triggered, this, &MainWindow::onActionSettings);
    connect(actionAbout, &QAction::triggered, this, &MainWindow::onActionAbout);
    connect(actionExit, &QAction::triggered, this, &MainWindow::onActionExit);
    connect(actionRussian, &QAction::triggered, this, &MainWindow::setLanguageRussian);
    connect(actionEnglish, &QAction::triggered, this, &MainWindow::setLanguageEnglish);
    connect(actionLightTheme, &QAction::triggered, this, &MainWindow::setThemeLight);
    connect(actionDarkTheme, &QAction::triggered, this, &MainWindow::setThemeDark);
    
    // Соединение сигналов от виджетов с обновлением статусной строки
    connect(taskWidget, &TaskWidget::taskListChanged, this, &MainWindow::updateStatusBar);
    connect(&timeTracker, &TimeTracker::timerStatusChanged, this, &MainWindow::updateStatusBar);
    connect(&pomodoroTimer, &PomodoroTimer::sessionStatusChanged, this, &MainWindow::updateStatusBar);
    
    // Соединение сигнала изменения вкладки с обновлением интерфейса
    connect(tabWidget, &QTabWidget::currentChanged, [this](int index) {
        // Обновление доступности действий в зависимости от активной вкладки
        bool isTaskTab = (index == 0);
        bool isTimeTrackerTab = (index == 1);
        bool isPomodoroTab = (index == 2);
        
        actionNewTask->setEnabled(isTaskTab);
        actionEditTask->setEnabled(isTaskTab);
        actionDeleteTask->setEnabled(isTaskTab);
        actionStartTimer->setEnabled(isTimeTrackerTab);
        actionPauseTimer->setEnabled(isTimeTrackerTab);
        actionStopTimer->setEnabled(isTimeTrackerTab);
        actionStartPomodoro->setEnabled(isPomodoroTab);
    });
}

void MainWindow::loadData()
{
    // Загрузка задач
    std::vector<Task> tasks = fileManager.loadTasks();
    for (const auto& task : tasks) {
        taskManager.addTask(task);
    }
    
    // Загрузка настроек Помодоро
    fileManager.loadSettings(pomodoroTimer);
    
    // Обновление интерфейса
    taskWidget->updateTaskList();
    statisticsWidget->updateStatistics();
}

void MainWindow::saveData()
{
    // Сохранение задач
    fileManager.saveTasks(taskManager.getAllTasks());
    
    // Сохранение настроек Помодоро
    fileManager.saveSettings(pomodoroTimer);
    
    // Сохранение настроек приложения
    settings.sync();
}

void MainWindow::changeLanguage(const QString &language)
{
    // Сохранение выбранного языка в настройках
    settings.setValue("app/language", language);
    
    // Загрузка соответствующего файла перевода
    QTranslator newTranslator;
    bool loaded = false;
    
    if (language == "ru") {
        // Пытаемся загрузить перевод из ресурсов
        loaded = newTranslator.load(":/translations/timemanager_ru.qm");
        
        // Если не удалось загрузить из ресурсов, пробуем загрузить из файловой системы
        if (!loaded) {
            QStringList paths = {
                QDir::currentPath() + "/translations",
                QDir::currentPath(),
                QApplication::applicationDirPath() + "/translations",
                QApplication::applicationDirPath()
            };
            
            for (const QString &path : paths) {
                if (newTranslator.load("timemanager_ru", path)) {
                    loaded = true;
                    break;
                }
            }
        }
    }
    
    // Удаляем текущий переводчик и устанавливаем новый
    QApplication::removeTranslator(&translator);
    
    if (loaded) {
        translator = newTranslator;
        QApplication::installTranslator(&translator);
    }
    
    // Обновление состояния действий меню
    actionRussian->setChecked(language == "ru");
    actionEnglish->setChecked(language == "en");
    
    // Обновление интерфейса
    updateStatusBar();
}

void MainWindow::changeTheme(const QString &theme)
{
    // Сохранение выбранной темы в настройках
    settings.setValue("app/theme", theme);
    
    // Загрузка соответствующего файла стилей
    QFile styleFile;
    
    if (theme == "dark") {
        styleFile.setFileName(":/styles/dark.qss");
    } else {
        styleFile.setFileName(":/styles/light.qss");
    }
    
    if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
        QString style = styleFile.readAll();
        qApp->setStyleSheet(style);
        styleFile.close();
    }
    
    // Обновление состояния действий меню
    actionDarkTheme->setChecked(theme == "dark");
    actionLightTheme->setChecked(theme == "light");
}

void MainWindow::updateStatusBar()
{
    // Обновление информации о задачах
    int totalTasks = taskManager.getTotalTaskCount();
    int completedTasks = taskManager.getCompletedTaskCount();
    int pendingTasks = taskManager.getPendingTaskCount();
    
    QString taskInfo = tr("Задачи: %1 всего, %2 выполнено, %3 ожидает")
                        .arg(totalTasks)
                        .arg(completedTasks)
                        .arg(pendingTasks);
    
    // Обновление информации о таймере
    QString timerInfo;
    if (timeTracker.getIsRunning()) {
        Task* currentTask = timeTracker.getCurrentTask();
        if (currentTask) {
            timerInfo = tr("Отслеживание: %1 - %2")
                        .arg(currentTask->getTitle().c_str())
                        .arg(QString::fromStdString(timeTracker.getElapsedTimeString()));
        }
    } else {
        timerInfo = tr("Отслеживание: неактивно");
    }
    
    // Обновление информации о Помодоро
    QString pomodoroInfo;
    if (pomodoroTimer.getIsRunning()) {
        pomodoroInfo = tr("Помодоро: %1 - %2")
                        .arg(QString::fromStdString(pomodoroTimer.getCurrentSessionTypeString()))
                        .arg(QString::fromStdString(pomodoroTimer.getRemainingTimeString()));
    } else {
        pomodoroInfo = tr("Помодоро: неактивно");
    }
    
    // Установка информации в статусную строку
    mainStatusBar->showMessage(taskInfo);
    
    // Обновление постоянных виджетов
    QLabel *timerLabel = qobject_cast<QLabel*>(mainStatusBar->children().at(1));
    if (timerLabel) {
        timerLabel->setText(timerInfo);
    }
    
    QLabel *pomodoroLabel = qobject_cast<QLabel*>(mainStatusBar->children().at(2));
    if (pomodoroLabel) {
        pomodoroLabel->setText(pomodoroInfo);
    }
}

// Слоты для действий меню

void MainWindow::onActionNewTask()
{
    taskWidget->createNewTask();
}

void MainWindow::onActionEditTask()
{
    taskWidget->editSelectedTask();
}

void MainWindow::onActionDeleteTask()
{
    taskWidget->deleteSelectedTask();
}

void MainWindow::onActionStartTimer()
{
    timeTrackerWidget->startTimer();
}

void MainWindow::onActionPauseTimer()
{
    timeTrackerWidget->pauseTimer();
}

void MainWindow::onActionStopTimer()
{
    timeTrackerWidget->stopTimer();
}

void MainWindow::onActionStartPomodoro()
{
    pomodoroWidget->startPomodoro();
}

void MainWindow::onActionSettings()
{
    SettingsDialog dialog(&pomodoroTimer, this);
    if (dialog.exec() == QDialog::Accepted) {
        // Обновление интерфейса после изменения настроек
        updateStatusBar();
    }
}

void MainWindow::onActionAbout()
{
    QMessageBox::about(this, tr("О программе"),
                      tr("<h3>Управление временем</h3>"
                         "<p>Версия 1.0.0</p>"
                         "<p>Приложение для управления временем и борьбы с прокрастинацией.</p>"
                         "<p>© 2025 NinjaTech</p>"));
}

void MainWindow::onActionExit()
{
    close();
}

void MainWindow::setLanguageRussian()
{
    changeLanguage("ru");
}

void MainWindow::setLanguageEnglish()
{
    changeLanguage("en");
}

void MainWindow::setThemeLight()
{
    changeTheme("light");
}

void MainWindow::setThemeDark()
{
    changeTheme("dark");
}