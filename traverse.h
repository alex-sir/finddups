/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#ifndef TRAVERSE
#define TRAVERSE

#include <stdio.h>
#include <stdlib.h>   // for exit
#include <dirent.h>   // for opendir, readdir
#include <unistd.h>   // for getcwd, stat
#include <sys/stat.h> // for struct stat
#include <limits.h>   // for _PC_PATH_MAX
#include <errno.h>    // for errno
#include <string.h>   // for strerror

// group of duplicate files
typedef struct
{
    int id;
    int members;
    char *pathnames[];
} Group;

extern DIR *getDir(const long PATHNAME_MAX);
extern void traverseDir(DIR *dir, Group *groups);

#endif
