#include "gui/SettingsDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QTabWidget>
#include <QFormLayout>
#include <QDir>
#include <QSettings>
#include <QApplication>
#include <QTranslator>
#include <QFile>
#include <QDateTime>

SettingsDialog::SettingsDialog(PomodoroTimer* pomodoroTimer, QWidget* parent)
    : QDialog(parent)
    , pomodoroTimer(pomodoroTimer)
    , tabWidget(new QTabWidget(this))
    , languageComboBox(new QComboBox(this))
    , themeComboBox(new QComboBox(this))
    , showNotificationsCheckBox(new QCheckBox(tr("Показывать уведомления"), this))
    , playSoundsCheckBox(new QCheckBox(tr("Воспроизводить звуки"), this))
    , startWithSystemCheckBox(new QCheckBox(tr("Запускать вместе с системой"), this))
    , minimizeToTrayCheckBox(new QCheckBox(tr("Сворачивать в трей"), this))
    , workDurationSpinBox(new QSpinBox(this))
    , breakDurationSpinBox(new QSpinBox(this))
    , longBreakDurationSpinBox(new QSpinBox(this))
    , sessionsBeforeLongBreakSpinBox(new QSpinBox(this))
    , autoStartBreaksCheckBox(new QCheckBox(tr("Автоматически начинать перерывы"), this))
    , autoStartWorkSessionsCheckBox(new QCheckBox(tr("Автоматически начинать рабочие сессии"), this))
    , dataDirectoryEdit(new QLineEdit(this))
    , browseButton(new QPushButton(tr("Обзор..."), this))
    , autoSaveCheckBox(new QCheckBox(tr("Автосохранение"), this))
    , autoSaveIntervalSpinBox(new QSpinBox(this))
    , backupButton(new QPushButton(tr("Резервное копирование"), this))
    , restoreButton(new QPushButton(tr("Восстановить из резервной копии"), this))
    , restoreDefaultsButton(new QPushButton(tr("Восстановить настройки по умолчанию"), this))
{
    setWindowTitle(tr("Настройки"));
    setupUi();
    loadSettings();
    setupConnections();
}

void SettingsDialog::setupUi()
{
    // Создание компоновки
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Создание вкладок
    tabWidget->addTab(createGeneralTab(), tr("Общие"));
    tabWidget->addTab(createPomodoroTab(), tr("Помодоро"));
    tabWidget->addTab(createDataTab(), tr("Данные"));
    
    // Создание кнопок диалога
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &SettingsDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);
    
    // Добавление всех элементов в основную компоновку
    mainLayout->addWidget(tabWidget);
    mainLayout->addWidget(buttonBox);
    
    setLayout(mainLayout);
    resize(500, 400);
}

QWidget* SettingsDialog::createGeneralTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Группа языка и темы
    QGroupBox* appearanceGroup = new QGroupBox(tab);
    QFormLayout* appearanceLayout = new QFormLayout(appearanceGroup);
    
    // Настройка языка
    QLabel* languageLabel = new QLabel(tr("Язык:"), appearanceGroup);
    languageComboBox->addItem("English", "en");
    languageComboBox->addItem("Русский", "ru");
    appearanceLayout->addRow(languageLabel, languageComboBox);
    
    // Настройка темы
    QLabel* themeLabel = new QLabel(tr("Тема:"), appearanceGroup);
    themeComboBox->addItem(tr("Светлая"), "light");
    themeComboBox->addItem(tr("Тёмная"), "dark");
    appearanceLayout->addRow(themeLabel, themeComboBox);
    
    // Группа уведомлений
    QGroupBox* notificationsGroup = new QGroupBox(tab);
    QVBoxLayout* notificationsLayout = new QVBoxLayout(notificationsGroup);
    notificationsLayout->addWidget(showNotificationsCheckBox);
    notificationsLayout->addWidget(playSoundsCheckBox);
    
    // Группа системных настроек
    QGroupBox* systemGroup = new QGroupBox(tab);
    QVBoxLayout* systemLayout = new QVBoxLayout(systemGroup);
    systemLayout->addWidget(startWithSystemCheckBox);
    systemLayout->addWidget(minimizeToTrayCheckBox);
    
    // Добавление всех групп в компоновку вкладки
    layout->addWidget(appearanceGroup);
    layout->addWidget(notificationsGroup);
    layout->addWidget(systemGroup);
    layout->addStretch();
    
    return tab;
}

QWidget* SettingsDialog::createPomodoroTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Группа настроек Помодоро
    QGroupBox* pomodoroGroup = new QGroupBox(tab);
    QFormLayout* pomodoroLayout = new QFormLayout(pomodoroGroup);
    
    // Настройка продолжительности работы
    QLabel* workDurationLabel = new QLabel(tr("Продолжительность работы (минуты):"), pomodoroGroup);
    workDurationSpinBox->setRange(1, 60);
    workDurationSpinBox->setValue(25);
    workDurationSpinBox->setSuffix(tr(" мин"));
    pomodoroLayout->addRow(workDurationLabel, workDurationSpinBox);
    
    // Настройка продолжительности перерыва
    QLabel* breakDurationLabel = new QLabel(tr("Продолжительность перерыва (минуты):"), pomodoroGroup);
    breakDurationSpinBox->setRange(1, 30);
    breakDurationSpinBox->setValue(5);
    breakDurationSpinBox->setSuffix(tr(" мин"));
    pomodoroLayout->addRow(breakDurationLabel, breakDurationSpinBox);
    
    // Настройка продолжительности длинного перерыва
    QLabel* longBreakDurationLabel = new QLabel(tr("Продолжительность длинного перерыва (минуты):"), pomodoroGroup);
    longBreakDurationSpinBox->setRange(5, 60);
    longBreakDurationSpinBox->setValue(15);
    longBreakDurationSpinBox->setSuffix(tr(" мин"));
    pomodoroLayout->addRow(longBreakDurationLabel, longBreakDurationSpinBox);
    
    // Настройка количества сессий перед длинным перерывом
    QLabel* sessionsBeforeLongBreakLabel = new QLabel(tr("Сессий перед длинным перерывом:"), pomodoroGroup);
    sessionsBeforeLongBreakSpinBox->setRange(1, 10);
    sessionsBeforeLongBreakSpinBox->setValue(4);
    pomodoroLayout->addRow(sessionsBeforeLongBreakLabel, sessionsBeforeLongBreakSpinBox);
    
    // Группа автоматизации
    QGroupBox* automationGroup = new QGroupBox(tab);
    QVBoxLayout* automationLayout = new QVBoxLayout(automationGroup);
    automationLayout->addWidget(autoStartBreaksCheckBox);
    automationLayout->addWidget(autoStartWorkSessionsCheckBox);
    
    // Добавление всех групп в компоновку вкладки
    layout->addWidget(pomodoroGroup);
    layout->addWidget(automationGroup);
    layout->addStretch();
    
    return tab;
}

QWidget* SettingsDialog::createDataTab()
{
    QWidget* tab = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(tab);
    
    // Группа директории данных
    QGroupBox* dataDirectoryGroup = new QGroupBox(tab);
    QFormLayout* dataDirectoryLayout = new QFormLayout(dataDirectoryGroup);
    
    // Настройка директории данных
    QLabel* dataDirectoryLabel = new QLabel(tr("Директория данных:"), dataDirectoryGroup);
    QHBoxLayout* directoryLayout = new QHBoxLayout();
    directoryLayout->addWidget(dataDirectoryEdit);
    directoryLayout->addWidget(browseButton);
    dataDirectoryLayout->addRow(dataDirectoryLabel, directoryLayout);
    
    // Группа автосохранения
    QGroupBox* autoSaveGroup = new QGroupBox(tab);
    QVBoxLayout* autoSaveLayout = new QVBoxLayout(autoSaveGroup);
    autoSaveLayout->addWidget(autoSaveCheckBox);
    
    QHBoxLayout* autoSaveIntervalLayout = new QHBoxLayout();
    QLabel* autoSaveIntervalLabel = new QLabel(tr("Интервал автосохранения (минуты):"), autoSaveGroup);
    autoSaveIntervalSpinBox->setRange(1, 60);
    autoSaveIntervalSpinBox->setValue(5);
    autoSaveIntervalSpinBox->setSuffix(tr(" мин"));
    autoSaveIntervalLayout->addWidget(autoSaveIntervalLabel);
    autoSaveIntervalLayout->addWidget(autoSaveIntervalSpinBox);
    autoSaveLayout->addLayout(autoSaveIntervalLayout);
    
    // Группа резервного копирования
    QGroupBox* backupGroup = new QGroupBox(tab);
    QVBoxLayout* backupLayout = new QVBoxLayout(backupGroup);
    backupLayout->addWidget(backupButton);
    backupLayout->addWidget(restoreButton);
    
    // Кнопка восстановления настроек по умолчанию
    QHBoxLayout* defaultsLayout = new QHBoxLayout();
    defaultsLayout->addStretch();
    defaultsLayout->addWidget(restoreDefaultsButton);
    
    // Добавление всех групп в компоновку вкладки
    layout->addWidget(dataDirectoryGroup);
    layout->addWidget(autoSaveGroup);
    layout->addWidget(backupGroup);
    layout->addLayout(defaultsLayout);
    layout->addStretch();
    
    return tab;
}

void SettingsDialog::setupConnections()
{
    // Соединение сигналов и слотов
    connect(browseButton, &QPushButton::clicked, this, &SettingsDialog::browseDataDirectory);
    connect(backupButton, &QPushButton::clicked, this, &SettingsDialog::backupData);
    connect(restoreButton, &QPushButton::clicked, this, &SettingsDialog::restoreFromBackup);
    connect(restoreDefaultsButton, &QPushButton::clicked, this, &SettingsDialog::restoreDefaults);
    
    connect(autoSaveCheckBox, &QCheckBox::toggled, autoSaveIntervalSpinBox, &QSpinBox::setEnabled);
}

void SettingsDialog::loadSettings()
{
    QSettings settings;
    
    // Загрузка общих настроек
    QString language = settings.value("app/language", QLocale::system().name()).toString();
    int languageIndex = languageComboBox->findData(language.left(2));
    if (languageIndex != -1) {
        languageComboBox->setCurrentIndex(languageIndex);
    }
    
    QString theme = settings.value("app/theme", "light").toString();
    int themeIndex = themeComboBox->findData(theme);
    if (themeIndex != -1) {
        themeComboBox->setCurrentIndex(themeIndex);
    }
    
    showNotificationsCheckBox->setChecked(settings.value("app/showNotifications", true).toBool());
    playSoundsCheckBox->setChecked(settings.value("app/playSounds", true).toBool());
    startWithSystemCheckBox->setChecked(settings.value("app/startWithSystem", false).toBool());
    minimizeToTrayCheckBox->setChecked(settings.value("app/minimizeToTray", true).toBool());
    
    // Загрузка настроек Помодоро
    workDurationSpinBox->setValue(pomodoroTimer->getWorkDuration());
    breakDurationSpinBox->setValue(pomodoroTimer->getBreakDuration());
    longBreakDurationSpinBox->setValue(pomodoroTimer->getLongBreakDuration());
    sessionsBeforeLongBreakSpinBox->setValue(pomodoroTimer->getSessionsBeforeLongBreak());
    
    autoStartBreaksCheckBox->setChecked(settings.value("pomodoro/autoStartBreaks", true).toBool());
    autoStartWorkSessionsCheckBox->setChecked(settings.value("pomodoro/autoStartWorkSessions", false).toBool());
    
    // Загрузка настроек данных
    QString dataDir = settings.value("data/directory", QDir::homePath() + "/TimeManagementApp").toString();
    dataDirectoryEdit->setText(dataDir);
    
    bool autoSave = settings.value("data/autoSave", true).toBool();
    autoSaveCheckBox->setChecked(autoSave);
    
    int autoSaveInterval = settings.value("data/autoSaveInterval", 5).toInt();
    autoSaveIntervalSpinBox->setValue(autoSaveInterval);
    autoSaveIntervalSpinBox->setEnabled(autoSave);
}

void SettingsDialog::saveSettings()
{
    QSettings settings;
    
    // Сохранение общих настроек
    settings.setValue("app/language", languageComboBox->currentData().toString());
    settings.setValue("app/theme", themeComboBox->currentData().toString());
    settings.setValue("app/showNotifications", showNotificationsCheckBox->isChecked());
    settings.setValue("app/playSounds", playSoundsCheckBox->isChecked());
    settings.setValue("app/startWithSystem", startWithSystemCheckBox->isChecked());
    settings.setValue("app/minimizeToTray", minimizeToTrayCheckBox->isChecked());
    
    // Сохранение настроек Помодоро
    pomodoroTimer->setWorkDuration(workDurationSpinBox->value());
    pomodoroTimer->setBreakDuration(breakDurationSpinBox->value());
    pomodoroTimer->setLongBreakDuration(longBreakDurationSpinBox->value());
    pomodoroTimer->setSessionsBeforeLongBreak(sessionsBeforeLongBreakSpinBox->value());
    
    settings.setValue("pomodoro/workDuration", workDurationSpinBox->value());
    settings.setValue("pomodoro/breakDuration", breakDurationSpinBox->value());
    settings.setValue("pomodoro/longBreakDuration", longBreakDurationSpinBox->value());
    settings.setValue("pomodoro/sessionsBeforeLongBreak", sessionsBeforeLongBreakSpinBox->value());
    settings.setValue("pomodoro/autoStartBreaks", autoStartBreaksCheckBox->isChecked());
    settings.setValue("pomodoro/autoStartWorkSessions", autoStartWorkSessionsCheckBox->isChecked());
    
    // Сохранение настроек данных
    settings.setValue("data/directory", dataDirectoryEdit->text());
    settings.setValue("data/autoSave", autoSaveCheckBox->isChecked());
    settings.setValue("data/autoSaveInterval", autoSaveIntervalSpinBox->value());
    
    // Применение изменений языка и темы
    applyLanguageChange();
    applyThemeChange();
}

void SettingsDialog::accept()
{
    saveSettings();
    QDialog::accept();
}

void SettingsDialog::browseDataDirectory()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Выберите директорию данных"),
                                                   dataDirectoryEdit->text(),
                                                   QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    
    if (!dir.isEmpty()) {
        dataDirectoryEdit->setText(dir);
    }
}

void SettingsDialog::backupData()
{
    QString backupDir = QFileDialog::getExistingDirectory(this, tr("Выберите директорию для резервной копии"),
                                                         QDir::homePath(),
                                                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    
    if (backupDir.isEmpty()) {
        return;
    }
    
    QString dataDir = dataDirectoryEdit->text();
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString backupFileName = QString("%1/TimeManagementApp_backup_%2.zip").arg(backupDir).arg(timestamp);
    
    // Создание архива с данными
    QStringList args;
    args << "-r" << backupFileName << dataDir;
    
    QProcess zipProcess;
    zipProcess.start("zip", args);
    
    if (!zipProcess.waitForFinished(30000)) {
        QMessageBox::warning(this, tr("Ошибка резервного копирования"), tr("Не удалось создать резервную копию: %1").arg(zipProcess.errorString()));
        return;
    }
    
    if (zipProcess.exitCode() != 0) {
        QMessageBox::warning(this, tr("Ошибка резервного копирования"), tr("Не удалось создать резервную копию: код ошибки %1").arg(zipProcess.exitCode()));
        return;
    }
    
    QMessageBox::information(this, tr("Резервная копия создана"), tr("Резервная копия успешно создана в %1").arg(backupFileName));
}

void SettingsDialog::restoreFromBackup()
{
    QString backupFile = QFileDialog::getOpenFileName(this, tr("Выберите файл резервной копии"),
                                                     QDir::homePath(),
                                                     tr("Файлы резервных копий (*.zip)"));
    
    if (backupFile.isEmpty()) {
        return;
    }
    
    QString dataDir = dataDirectoryEdit->text();
    
    // Подтверждение восстановления
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Подтверждение восстановления"),
                                                             tr("Восстановление из резервной копии заменит все текущие данные. Продолжить?"),
                                                             QMessageBox::Yes | QMessageBox::No);
    
    if (reply != QMessageBox::Yes) {
        return;
    }
    
    // Создание временной директории для распаковки
    QString tempDir = QDir::tempPath() + "/TimeManagementApp_restore_" + QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QDir().mkpath(tempDir);
    
    // Распаковка архива
    QStringList args;
    args << "-o" << backupFile << "-d" << tempDir;
    
    QProcess unzipProcess;
    unzipProcess.start("unzip", args);
    
    if (!unzipProcess.waitForFinished(30000)) {
        QMessageBox::warning(this, tr("Ошибка восстановления"), tr("Не удалось распаковать резервную копию: %1").arg(unzipProcess.errorString()));
        return;
    }
    
    if (unzipProcess.exitCode() != 0) {
        QMessageBox::warning(this, tr("Ошибка восстановления"), tr("Не удалось распаковать резервную копию: код ошибки %1").arg(unzipProcess.exitCode()));
        return;
    }
    
    // Копирование данных из временной директории
    QStringList copyArgs;
    copyArgs << "-r" << tempDir + "/*" << dataDir;
    
    QProcess copyProcess;
    copyProcess.start("cp", copyArgs);
    
    if (!copyProcess.waitForFinished(30000)) {
        QMessageBox::warning(this, tr("Ошибка восстановления"), tr("Не удалось скопировать данные: %1").arg(copyProcess.errorString()));
        return;
    }
    
    if (copyProcess.exitCode() != 0) {
        QMessageBox::warning(this, tr("Ошибка восстановления"), tr("Не удалось скопировать данные: код ошибки %1").arg(copyProcess.exitCode()));
        return;
    }
    
    // Удаление временной директории
    QStringList rmArgs;
    rmArgs << "-rf" << tempDir;
    
    QProcess rmProcess;
    rmProcess.start("rm", rmArgs);
    rmProcess.waitForFinished();
    
    QMessageBox::information(this, tr("Восстановление успешно"), tr("Данные успешно восстановлены из резервной копии. Приложение будет перезапущено."));
    
    // Перезапуск приложения
    QProcess::startDetached(QApplication::applicationFilePath(), QStringList());
    QApplication::quit();
}

void SettingsDialog::restoreDefaults()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, tr("Подтверждение сброса"),
                                                             tr("Вы уверены, что хотите восстановить настройки по умолчанию? Это действие нельзя отменить."),
                                                             QMessageBox::Yes | QMessageBox::No);
    
    if (reply != QMessageBox::Yes) {
        return;
    }
    
    // Сброс общих настроек
    languageComboBox->setCurrentIndex(languageComboBox->findData("en"));
    themeComboBox->setCurrentIndex(themeComboBox->findData("light"));
    showNotificationsCheckBox->setChecked(true);
    playSoundsCheckBox->setChecked(true);
    startWithSystemCheckBox->setChecked(false);
    minimizeToTrayCheckBox->setChecked(true);
    
    // Сброс настроек Помодоро
    workDurationSpinBox->setValue(25);
    breakDurationSpinBox->setValue(5);
    longBreakDurationSpinBox->setValue(15);
    sessionsBeforeLongBreakSpinBox->setValue(4);
    autoStartBreaksCheckBox->setChecked(true);
    autoStartWorkSessionsCheckBox->setChecked(false);
    
    // Сброс настроек данных
    dataDirectoryEdit->setText(QDir::homePath() + "/TimeManagementApp");
    autoSaveCheckBox->setChecked(true);
    autoSaveIntervalSpinBox->setValue(5);
    autoSaveIntervalSpinBox->setEnabled(true);
}

void SettingsDialog::applyLanguageChange()
{
    QString newLanguage = languageComboBox->currentData().toString();
    QString currentLanguage = QSettings().value("app/language", "en").toString();
    
    if (newLanguage != currentLanguage) {
        // Загрузка нового перевода
        QTranslator translator;
        bool loaded = false;
        
        if (newLanguage == "ru") {
            // Попытка загрузить перевод из ресурсов
            loaded = translator.load(":/translations/timemanager_ru.qm");
            
            // Если не удалось загрузить из ресурсов, пробуем загрузить из файловой системы
            if (!loaded) {
                QStringList paths = {
                    QDir::currentPath() + "/translations",
                    QDir::currentPath(),
                    QApplication::applicationDirPath() + "/translations",
                    QApplication::applicationDirPath()
                };
                
                for (const QString &path : paths) {
                    if (translator.load("timemanager_ru", path)) {
                        loaded = true;
                        break;
                    }
                }
            }
        }
        
        // Удаляем текущий переводчик и устанавливаем новый
        QApplication::removeTranslator(QApplication::instance()->findChild<QTranslator*>());
        
        if (loaded) {
            QTranslator* appTranslator = new QTranslator(QApplication::instance());
            *appTranslator = translator;
            QApplication::installTranslator(appTranslator);
        }
    }
}

void SettingsDialog::applyThemeChange()
{
    QString newTheme = themeComboBox->currentData().toString();
    QString currentTheme = QSettings().value("app/theme", "light").toString();
    
    if (newTheme != currentTheme) {
        // Загрузка нового стиля
        QFile styleFile;
        
        if (newTheme == "dark") {
            styleFile.setFileName(":/styles/dark.qss");
        } else {
            styleFile.setFileName(":/styles/light.qss");
        }
        
        if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
            QString style = styleFile.readAll();
            qApp->setStyleSheet(style);
            styleFile.close();
        }
    }
}