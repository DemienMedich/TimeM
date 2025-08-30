#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QTabWidget>
#include <QSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QGroupBox>
#include <QLabel>
#include <QDialogButtonBox>
#include <QSettings>

#include "../core/PomodoroTimer.h"

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(PomodoroTimer *pomodoroTimer, QWidget *parent = nullptr);
    ~SettingsDialog();

private slots:
    // Слоты для обработки действий пользователя
    void onAccepted();
    void onRejected();
    void onRestoreDefaultsClicked();
    
    // Слоты для изменения настроек
    void onLanguageChanged(int index);
    void onThemeChanged(int index);
    void onNotificationsToggled(bool checked);
    void onSoundToggled(bool checked);
    void onAutoStartToggled(bool checked);
    void onMinimizeToTrayToggled(bool checked);

private:
    // Настройка интерфейса
    void setupUi();
    void setupConnections();
    
    // Загрузка и сохранение настроек
    void loadSettings();
    void saveSettings();
    void restoreDefaults();
    
    // Компоненты интерфейса
    QTabWidget *tabWidget;
    
    // Вкладка "Общие"
    QWidget *generalTab;
    QComboBox *languageComboBox;
    QComboBox *themeComboBox;
    QCheckBox *notificationsCheckBox;
    QCheckBox *soundCheckBox;
    QCheckBox *autoStartCheckBox;
    QCheckBox *minimizeToTrayCheckBox;
    
    // Вкладка "Помодоро"
    QWidget *pomodoroTab;
    QSpinBox *workDurationSpinBox;
    QSpinBox *breakDurationSpinBox;
    QSpinBox *longBreakDurationSpinBox;
    QSpinBox *sessionsBeforeLongBreakSpinBox;
    QCheckBox *autoStartBreakCheckBox;
    QCheckBox *autoStartWorkCheckBox;
    
    // Вкладка "Данные"
    QWidget *dataTab;
    QLineEdit *dataDirectoryEdit;
    QPushButton *browseButton;
    QCheckBox *autoSaveCheckBox;
    QSpinBox *autoSaveIntervalSpinBox;
    QPushButton *backupButton;
    QPushButton *restoreButton;
    
    // Кнопки диалога
    QDialogButtonBox *buttonBox;
    QPushButton *restoreDefaultsButton;
    
    // Ссылка на таймер Помодоро
    PomodoroTimer *pomodoroTimer;
    
    // Настройки приложения
    QSettings settings;
};

#endif // SETTINGSDIALOG_H