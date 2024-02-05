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
#include <string.h>   // for strerror, strcmp

#define PATHNAME_MAX pathconf(".", _PC_PATH_MAX) // max number of bytes in a file pathname

// group of duplicate files
typedef struct
{
    int id;
    int members;
    char *pathnames[];
} Group;

/**
 * @brief gets the name of the current directory
 *
 * @return char* name of the current directory
 */
extern char *getCurDir(void);
extern void traverseDir(char *dir, char *parentDir, Group *groups);
extern void checkDupsFile(struct dirent *file, struct stat fileInfo, char *filePathname, char *dirName, Group *groups);
extern int compareFiles(char *pathname1, char *pathname2);

#endif
