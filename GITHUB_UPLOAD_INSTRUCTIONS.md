# Инструкции по загрузке проекта на GitHub

## 1. Подготовка

### Установка Git (если еще не установлен)
```bash
# Для Debian/Ubuntu
sudo apt-get update
sudo apt-get install git

# Для macOS (с использованием Homebrew)
brew install git

# Для Windows
# Скачайте и установите Git с https://git-scm.com/download/win
```

### Настройка Git
```bash
git config --global user.name "Ваше Имя"
git config --global user.email "ваша.почта@example.com"
```

### Инициализация репозитория
Перейдите в директорию проекта и выполните:
```bash
git init
```

### Создание .gitignore файла
Создайте файл .gitignore в корне проекта со следующим содержимым:
```
# Игнорировать файлы сборки
build/
*.o
*.obj
*.exe
*.out
*.app

# Игнорировать файлы CMake
CMakeFiles/
CMakeCache.txt
cmake_install.cmake
Makefile
*.cmake

# Игнорировать файлы IDE
.vscode/
.idea/
*.swp
*.swo

# Игнорировать файлы данных приложения
*.dat
*.bak
```

## 2. Создание репозитория на GitHub

1. Перейдите на [GitHub](https://github.com/) и войдите в свой аккаунт
2. Нажмите на "+" в правом верхнем углу и выберите "New repository"
3. Введите имя репозитория: "TimeManagementApp"
4. Добавьте описание: "C++ application for time management and fighting procrastination"
5. Выберите "Public" (или "Private", если хотите сделать репозиторий приватным)
6. НЕ инициализируйте репозиторий с README, .gitignore или лицензией
7. Нажмите "Create repository"

## 3. Загрузка проекта

После создания репозитория GitHub покажет инструкции. Выполните следующие команды в директории проекта:

```bash
# Добавить все файлы в индекс
git add .

# Сделать первый коммит
git commit -m "Initial commit: Time Management and Anti-Procrastination Application"

# Связать локальный репозиторий с удаленным (замените YOUR_USERNAME на ваше имя пользователя GitHub)
git remote add origin https://github.com/YOUR_USERNAME/TimeManagementApp.git

# Отправить изменения на GitHub
git push -u origin master
```

## 4. Проверка загрузки

1. Перейдите на страницу вашего репозитория на GitHub
2. Убедитесь, что все файлы проекта успешно загружены
3. Проверьте README.md, который должен отображаться на главной странице репозитория

## 5. Дополнительные шаги (опционально)

### Добавление релиза
1. На странице репозитория перейдите в раздел "Releases"
2. Нажмите "Create a new release"
3. Введите тег версии (например, "v1.0.0")
4. Добавьте название и описание релиза
5. При желании загрузите скомпилированные бинарные файлы
6. Нажмите "Publish release"

### Настройка GitHub Pages (для документации)
Если вы хотите создать веб-страницу с документацией:
1. Перейдите в настройки репозитория
2. Прокрутите вниз до раздела "GitHub Pages"
3. Выберите ветку (обычно "master") и папку (обычно "docs" или "root")
4. Нажмите "Save"

## 6. Обновление проекта в будущем

Для внесения изменений в проект и их загрузки на GitHub:

```bash
# Внесите изменения в файлы проекта

# Добавьте измененные файлы в индекс
git add .

# Создайте коммит с описанием изменений
git commit -m "Описание внесенных изменений"

# Отправьте изменения на GitHub
git push origin master
```