#include <stdio.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

void checkPermission(char *path, struct passwd *user, struct group *group) {
    //Открытие и чтение переданного файла
    DIR *d;
    struct dirent *dir;
    if((d = opendir(path)) == NULL)
        perror("line 17");

    while ((dir = readdir(d)) != NULL) {
        //Исключение директорий .. и .
        if ( 0 != strcmp(dir->d_name, "..") && 0 != strcmp(dir->d_name, ".") ) {
            // Сохранение пути
            char currentPath[PATH_MAX];
            for (int i = 0; i < PATH_MAX; ++i) {
                currentPath[i] = 0;
            }
            strcat(currentPath, path);
            strcat(currentPath, dir->d_name);

            // Получение данных о файле
            struct stat statbuf;
            stat(currentPath, &statbuf);
            mode_t modes = statbuf.st_mode;

            // Проверка условия
            int permission;
            if(statbuf.st_uid == user->pw_uid)
                permission = (modes & S_IWUSR);
            else if (statbuf.st_gid == group->gr_gid)
                permission = (modes & S_IWGRP);
            else
                permission = (modes & S_IWOTH);

            if(permission)
            {
                //Проверка на тип, директория или нет
                if(S_ISDIR(modes)){
                    strcat(currentPath , "/");
                    checkPermission(currentPath, user, group);
                    printf("d %s \n", currentPath);
                }
                else printf("f %s \n", currentPath);
            }
        }
    }
    closedir(d);
}

void parseArguments(int argc, char **argv, char **path, char **name, char  **group) {
    //Обработка каждого аргумента
    int opt = 0;
    while ((opt = getopt(argc, argv, "u:g:p:")) != -1) {
        switch (opt) {
            case 'u':
                *name = optarg;
                break;
            case 'g':
                *group = optarg;
                break;
            case 'p':
                *path = optarg;
                break;
        }
    }
}

int main(int argc, char **argv){
    //Инициализация переменных
    char* userName;
    char* groupName;
    char* path;
    struct passwd *user;
    struct group *group;
    int permission;

    //Парсит аргументы
    parseArguments(argc, argv, &path, &userName, &groupName);

    //Получение данных о пользователе и группе
    user = getpwnam(userName);
    group = getgrnam(groupName);

    //Получение данных о файле
    struct stat statbuf;
    stat(path, &statbuf);
    mode_t modes = statbuf.st_mode;

    //Проверка условия или владелец или группа или все
    if(statbuf.st_uid == user->pw_uid)
        permission = (modes & S_IWUSR);
    else if (statbuf.st_gid == group->gr_gid)
        permission = (modes & S_IWGRP);
    else
        permission = (modes & S_IWOTH);

    if(permission)
    {
            //Проверка на тип, директория или нет
            if(S_ISDIR(modes)){
                checkPermission(path,user, group);
                printf("d %s \n", path);
            }
            else printf("f %s \n", path);
    }
    exit(0);
}