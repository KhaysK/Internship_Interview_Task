#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

char* userName;
char* groupName;
char* path;
struct passwd *user;
struct group *group;
struct stat statbuf;

void parseArguments(int argc, char **argv) {
    int opt = 0;
    while ((opt = getopt(argc, argv, "u:g:p:")) != -1) {
        switch (opt) {
            case 'u':
                userName = optarg;
                break;
            case 'g':
                groupName = optarg;
                break;
            case 'p':
                path = optarg;
                break;
        }
    }
}
void checkPermission(char *path) {

    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    int counter = 0;
    while ((dir = readdir(d)) != NULL) {
        stat(dir->d_name, &statbuf);
        mode_t modes = statbuf.st_mode;
        if (counter > -1) {
            if((statbuf.st_uid == user->pw_uid) ? (modes & S_IWUSR) && ((statbuf.st_gid == group->gr_gid) && (modes & S_IWGRP)
            || (statbuf.st_gid == group->gr_gid)
            ? ((modes & S_IWGRP) && (modes & S_IWOTH)) || (modes & S_IWGRP) : (modes & S_IWOTH)) || (modes & S_IWUSR)
            : ((statbuf.st_gid == group->gr_gid) && (modes & S_IWGRP) || (statbuf.st_gid == group->gr_gid)
            ? ((modes & S_IWGRP) && (modes & S_IWOTH)) || (modes & S_IWGRP) : (modes & S_IWOTH))){
                if (S_ISDIR(modes)) {
                    getcwd(path, FILENAME_MAX);
                    strcat(path, dir->d_name);
                    strcat(path, "/");
                            //checkPermission(dir->d_name);
                    printf("%s \n", path);
                }
                else{
                    getcwd(path, FILENAME_MAX);
                    strcat(path, "/");
                    strcat(path, dir->d_name);
                    printf("%s sds\n", path);
                }
            }
        }
        counter++;
    }
    closedir(d);
}

int main(int argc, char **argv){
    parseArguments(argc, argv);

    user = getpwnam(userName);
    group = getgrnam(groupName);
    stat(path, &statbuf);
    mode_t modes = statbuf.st_mode;

    if((statbuf.st_uid == user->pw_uid) ? (modes & S_IWUSR) && ((statbuf.st_gid == group->gr_gid) && (modes & S_IWGRP) || (statbuf.st_gid == group->gr_gid)
        ? ((modes & S_IWGRP) && (modes & S_IWOTH)) || (modes & S_IWGRP) : (modes & S_IWOTH)) || (modes & S_IWUSR)
        : ((statbuf.st_gid == group->gr_gid) && (modes & S_IWGRP) || (statbuf.st_gid == group->gr_gid)
        ? ((modes & S_IWGRP) && (modes & S_IWOTH)) || (modes & S_IWGRP) : (modes & S_IWOTH))){
            if(S_ISDIR(modes)){
                checkPermission(path);
                printf("%s \n", path);
            }
            else
                printf("%s \n", path);
    }
    exit(0);
}