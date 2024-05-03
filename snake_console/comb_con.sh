#!/bin/bash

# Создаем текстовый файл и очищаем его
output_file="combined_files.txt"
> "$output_file"

# Массив файлов для комбинирования
files_to_combine=(
    "snake_console.cpp"
    "../brick_game/snake/game.h"
    "../brick_game/snake/game.cpp"
    "../gui/desktop/console_controller.h"
    "../gui/desktop/console_controller.cpp"
    "../gui/desktop/console_view.h"
    "../gui/desktop/console_view.cpp"
    # Добавьте остальные файлы, которые вам нужно комбинировать
)

# Собираем содержимое указанных файлов в текущей папке
for file in "${files_to_combine[@]}"; do
    # Проверяем, является ли файл действительным файлом
    if [ -f "$file" ]; then
        # Добавляем название файла в созданный текстовый файл
        echo "=== $file ===" >> "$output_file"
        # Добавляем содержимое файла в созданный текстовый файл
        cat "$file" >> "$output_file"
        echo "" >> "$output_file" # добавляем пустую строку для разделения файлов
    else
        echo "Предупреждение: Файл '$file' не найден или не является файлом."
    fi
done

echo "Готово! Содержимое указанных файлов объединено в файл $output_file"