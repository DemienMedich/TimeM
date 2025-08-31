# Инструкции по сборке приложения TimeManagementApp

В этом документе описаны шаги для сборки приложения TimeManagementApp как в консольной, так и в GUI версии.

## Требования

Для сборки приложения вам потребуются следующие компоненты:

### Для консольной версии:
- CMake 3.10 или выше
- Компилятор C++ с поддержкой C++17 (GCC 7+, Clang 5+, MSVC 2017+)

### Для GUI версии (дополнительно):
- Qt 5.15 или выше
- Qt Linguist Tools для компиляции файлов локализации

## Установка зависимостей

### Linux (Debian/Ubuntu)

```bash
# Установка базовых инструментов разработки
sudo apt-get update
sudo apt-get install -y build-essential cmake

# Установка Qt и инструментов для GUI версии
sudo apt-get install -y qtbase5-dev qttools5-dev-tools qtcharts5-dev
```

### Windows

1. Установите [CMake](https://cmake.org/download/)
2. Установите [Qt](https://www.qt.io/download) через Qt Online Installer
3. Установите [Visual Studio](https://visualstudio.microsoft.com/) с компонентами разработки C++

### macOS

```bash
# Установка через Homebrew
brew install cmake
brew install qt@5
```

## Сборка приложения

### Клонирование репозитория

```bash
git clone https://github.com/yourusername/TimeManagementApp.git
cd TimeManagementApp
```

### Создание директории для сборки

```bash
mkdir build
cd build
```

### Сборка консольной версии

```bash
cmake ..
make
```

### Сборка GUI версии

```bash
cmake .. -DBUILD_GUI=ON
make
```

На Windows с Visual Studio:

```bash
cmake .. -G "Visual Studio 16 2019" -A x64 -DBUILD_GUI=ON
cmake --build . --config Release
```

## Запуск приложения

### Консольная версия

```bash
./TimeManagementApp
```

### GUI версия

```bash
./TimeManagementAppGUI
```

## Компиляция файлов локализации

Если вы хотите обновить файлы локализации, выполните следующие шаги:

1. Обновите файлы .ts с помощью lupdate:

```bash
lupdate ../src -ts ../resources/translations/timemanager_ru.ts
```

2. Отредактируйте переводы с помощью Qt Linguist:

```bash
linguist ../resources/translations/timemanager_ru.ts
```

3. Скомпилируйте файлы .ts в .qm:

```bash
lrelease ../resources/translations/timemanager_ru.ts -qm ../resources/translations/timemanager_ru.qm
```

## Возможные проблемы и их решения

### Не найден Qt

Если CMake не может найти Qt, укажите путь к Qt явно:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/path/to/qt
```

### Ошибки компиляции, связанные с C++17

Убедитесь, что ваш компилятор поддерживает C++17. Для GCC версия должна быть не ниже 7, для Clang - не ниже 5.

### Проблемы с локализацией

Если приложение не отображает русский интерфейс:

1. Убедитесь, что файл timemanager_ru.qm находится в правильной директории (resources/translations/)
2. Проверьте, что файл правильно скомпилирован с помощью lrelease
3. В настройках приложения выберите русский язык

## Дополнительная информация

Для получения дополнительной информации о приложении и его использовании, обратитесь к файлу README.md и USAGE_GUIDE.md.