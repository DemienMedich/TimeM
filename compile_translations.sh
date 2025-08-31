#!/bin/bash
# Скрипт для компиляции файлов перевода .ts в .qm

# Проверяем наличие lrelease
if command -v lrelease &> /dev/null; then
    echo "Компиляция файлов перевода..."
    lrelease resources/translations/timemanager_ru.ts -qm resources/translations/timemanager_ru.qm
    echo "Компиляция завершена."
else
    echo "Ошибка: lrelease не найден. Убедитесь, что Qt установлен и находится в PATH."
    exit 1
fi