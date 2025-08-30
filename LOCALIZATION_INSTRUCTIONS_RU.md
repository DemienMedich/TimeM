# Инструкции по локализации приложения

## Обзор

Для полной локализации приложения на русский язык необходимо выполнить следующие шаги:

1. Заменить строки пользовательского интерфейса
2. Обновить комментарии в коде
3. Обновить документацию

## Шаги по локализации

### 1. Локализация пользовательского интерфейса

#### Вариант 1: Прямая замена строк в коде

1. Откройте файл `src/UserInterface.cpp`
2. Замените все английские строки на их русские эквиваленты из файла `localization_ru.txt`
3. Убедитесь, что форматирование строк сохранено (особенно для строк с форматированием, например `printf` или `std::cout`)

#### Вариант 2: Создание системы локализации

Для более гибкого подхода можно реализовать систему локализации:

1. Создайте класс `Localization`:

```cpp
// Localization.h
#ifndef LOCALIZATION_H
#define LOCALIZATION_H

#include <string>
#include <unordered_map>

class Localization {
private:
    std::unordered_map<std::string, std::string> translations;
    static Localization* instance;
    
    Localization();
    
public:
    static Localization* getInstance();
    
    bool loadTranslations(const std::string& filename);
    std::string getTranslation(const std::string& key) const;
};

#endif // LOCALIZATION_H
```

2. Реализуйте класс:

```cpp
// Localization.cpp
#include "Localization.h"
#include <fstream>
#include <iostream>

Localization* Localization::instance = nullptr;

Localization::Localization() {}

Localization* Localization::getInstance() {
    if (instance == nullptr) {
        instance = new Localization();
    }
    return instance;
}

bool Localization::loadTranslations(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    
    std::string line;
    while (std::getline(file, line)) {
        // Пропустить комментарии и пустые строки
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        // Найти разделитель "="
        size_t pos = line.find(" = ");
        if (pos != std::string::npos) {
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 3);
            
            // Удалить кавычки, если они есть
            if (key.front() == '"' && key.back() == '"') {
                key = key.substr(1, key.length() - 2);
            }
            if (value.front() == '"' && value.back() == '"') {
                value = value.substr(1, value.length() - 2);
            }
            
            translations[key] = value;
        }
    }
    
    file.close();
    return true;
}

std::string Localization::getTranslation(const std::string& key) const {
    auto it = translations.find(key);
    if (it != translations.end()) {
        return it->second;
    }
    return key; // Возвращаем исходный ключ, если перевод не найден
}
```

3. Добавьте макрос для упрощения использования:

```cpp
// В начале UserInterface.cpp
#define _(str) Localization::getInstance()->getTranslation(str)
```

4. Измените строки в коде:

```cpp
// Было
std::cout << "Task Management\n";

// Стало
std::cout << _("Управление задачами") << "\n";
```

5. Загрузите переводы при запуске:

```cpp
// В конструкторе UserInterface
Localization::getInstance()->loadTranslations("localization_ru.txt");
```

### 2. Локализация комментариев в коде

1. Используйте переведенные заголовочные файлы (`*_RU.h`) как справочные материалы
2. Постепенно обновляйте комментарии в исходных файлах

### 3. Локализация документации

1. Замените английские файлы документации на их русские версии:
   - `README.md` → `README_RU.md`
   - `USAGE_GUIDE.md` → `USAGE_GUIDE_RU.md`
   - `PROJECT_SUMMARY.md` → `PROJECT_SUMMARY_RU.md`
   - `GITHUB_UPLOAD_INSTRUCTIONS.md` → `GITHUB_UPLOAD_INSTRUCTIONS_RU.md`

2. Или создайте систему переключения языков в документации, добавив ссылки:

```markdown
# Time Management App

[English](README.md) | [Русский](README_RU.md)

...содержание документации...
```

## Поддержка нескольких языков

Для поддержки нескольких языков:

1. Создайте файлы локализации для каждого языка:
   - `localization_ru.txt` - для русского
   - `localization_en.txt` - для английского
   - и т.д.

2. Добавьте в приложение возможность выбора языка:

```cpp
// В меню настроек
void UserInterface::handleSettingsMenu() {
    // ...существующий код...
    
    std::cout << "4. Язык / Language\n";
    
    // ...обработка выбора...
    
    if (choice == 4) {
        std::cout << "1. Русский\n";
        std::cout << "2. English\n";
        
        int langChoice = getMenuChoice(2);
        if (langChoice == 1) {
            Localization::getInstance()->loadTranslations("localization_ru.txt");
        } else {
            Localization::getInstance()->loadTranslations("localization_en.txt");
        }
    }
}
```

## Проверка локализации

После внесения изменений:

1. Перекомпилируйте приложение
2. Проверьте все меню и функции на корректность отображения русского текста
3. Убедитесь, что форматирование текста не нарушено
4. Проверьте специальные символы и кодировку

## Возможные проблемы

1. **Проблемы с кодировкой**: Убедитесь, что все файлы сохранены в кодировке UTF-8
2. **Проблемы с отображением**: Некоторые консоли могут некорректно отображать кириллицу
3. **Длина строк**: Русские строки могут быть длиннее английских, что может нарушить форматирование

## Дополнительные улучшения

1. Добавьте поддержку других языков
2. Реализуйте сохранение выбранного языка в настройках
3. Добавьте автоматическое определение языка системы