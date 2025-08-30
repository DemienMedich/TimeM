#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QSettings>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QStyleFactory>
#include "gui/MainWindow.h"

int main(int argc, char *argv[])
{
    // Создаем приложение
    QApplication app(argc, argv);
    
    // Настройка информации о приложении
    QApplication::setApplicationName("TimeManagementApp");
    QApplication::setApplicationDisplayName("Управление временем");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("NinjaTech");
    QApplication::setOrganizationDomain("ninjatech.com");
    
    // Загрузка настроек
    QSettings settings;
    
    // Загрузка темы
    QString theme = settings.value("app/theme", "light").toString();
    if (theme == "dark") {
        QFile styleFile(":/styles/dark.qss");
        if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
            QString style = styleFile.readAll();
            app.setStyleSheet(style);
            styleFile.close();
        }
    } else {
        QFile styleFile(":/styles/light.qss");
        if (styleFile.open(QFile::ReadOnly | QFile::Text)) {
            QString style = styleFile.readAll();
            app.setStyleSheet(style);
            styleFile.close();
        }
    }
    
    // Загрузка локализации
    QString language = settings.value("app/language", QLocale::system().name()).toString();
    QTranslator translator;
    
    // Пытаемся загрузить перевод из ресурсов
    bool translationLoaded = false;
    if (language == "ru_RU" || language == "ru") {
        translationLoaded = translator.load(":/translations/timemanager_ru.qm");
    }
    
    // Если не удалось загрузить из ресурсов, пробуем загрузить из файловой системы
    if (!translationLoaded && (language == "ru_RU" || language == "ru")) {
        // Проверяем несколько возможных путей
        QStringList paths = {
            QDir::currentPath() + "/translations",
            QDir::currentPath(),
            QApplication::applicationDirPath() + "/translations",
            QApplication::applicationDirPath()
        };
        
        for (const QString &path : paths) {
            if (translator.load("timemanager_ru", path)) {
                translationLoaded = true;
                break;
            }
        }
    }
    
    // Если перевод загружен, устанавливаем его
    if (translationLoaded) {
        app.installTranslator(&translator);
    }
    
    // Создаем и показываем главное окно
    MainWindow mainWindow;
    mainWindow.show();
    
    // Запускаем цикл обработки событий
    return app.exec();
}