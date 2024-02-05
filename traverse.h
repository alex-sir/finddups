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

#define PATHNAME_MAX pathconf(".", _PC_PATH_MAX) + 1 // max number of bytes in a file pathname

// group of duplicate files
typedef struct
{
    int id;
    int count;
    char **pathnames;
} Group;

// group of groups
typedef struct
{
    int count;
    Group **members;
} Groups;

/**
 * @brief gets the name of the current directory
 *
 * @return char* name of the current directory
 */
extern char *getCurDir(void);
extern void traverseDir(const char *dir, const char *parentDir, Groups *groupsList);
int isAlreadyDup(Groups *groupsList, char *pathname);
void checkDupsFile(struct dirent *file, struct stat fileInfo, const char *filePathname, const char *dirName, Groups *groupsList);
Group *setupGroup(int groupsCount, char *pathname);
void updateGroup(Group *group, char *pathname);
int compareFiles(const char *pathname1, const char *pathname2);

#endif
