#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <locale.h>

int n=0;
// Функция для проверки типа файла
int match_type(const char *path, int type) {
    struct stat st;
    if (lstat(path, &st) != 0) {
        return 0;
    }
    switch (type) {
        case 'l': return S_ISLNK(st.st_mode); // Символическая ссылка
        case 'd': return S_ISDIR(st.st_mode); // Каталог
        case 'f': return S_ISREG(st.st_mode); // Обычный файл
        default: return 1; // Все типы
    }
}

// Рекурсивная функция для обхода каталога
void dirwalk(const char *dir, int options, int sort) {

    DIR *dp;
    struct dirent *entry;
    struct stat st;
    char path[1024];

    if ((dp = opendir(dir)) == NULL) {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue; // Пропускаем текущий и родительский каталоги
        }

        snprintf(path, sizeof(path), "%s/%s", dir, entry->d_name);

        if (match_type(path, options)) {
            n++;
            printf("%s\n", path); // Выводим путь, если он соответствует опциям
        }

        if (lstat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
            dirwalk(path, options, sort); // Рекурсивный вызов для подкаталогов
        }
    }

    closedir(dp);
}

int main(int argc, char *argv[]) {
    int opt;
    int options = 0; // По умолчанию выводятся все типы
    int sort = 0;    // Сортировка отключена
    char *dir = "."; // По умолчанию текущий каталог

    // Обработка опций с помощью getopt
    while ((opt = getopt(argc, argv, "ldfs")) != -1) {
        switch (opt) {
            case 'l': options = 'l'; break; // Только символические ссылки
            case 'd': options = 'd'; break; // Только каталоги
            case 'f': options = 'f'; break; // Только файлы
            case 's': sort = 1; break;      // Сортировка
            default:
                fprintf(stderr, "Использование: %s [dir] [-l] [-d] [-f] [-s]\n", argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    // Обработка аргумента каталога
    if (optind < argc) {
        dir = argv[optind];
    }

    // Установка локали для сортировки
    if (sort) {
        setlocale(LC_COLLATE, "");
    }

    // Запуск обхода каталога
    dirwalk(dir, options, sort);
    printf("objects found: %d\n",n);

    return 0;
}
