/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#ifndef TRAVERSE
#define TRAVERSE

#include <stdio.h>    // for print
#include <stdlib.h>   // for exit
#include <dirent.h>   // for opendir, readdir
#include <unistd.h>   // for getcwd, stat
#include <sys/stat.h> // for struct stat
#include <limits.h>   // for _PC_PATH_MAX
#include <errno.h>    // for errno
#include <string.h>   // for strerror, strcmp
#include <libgen.h>   // for dirname, basename

#define PATHNAME_MAX pathconf(".", _PC_PATH_MAX) + 1 // max number of bytes in a file pathname
#define FILE_DIR 1
#define FILE_REG 2
#define FILE_INV -1
#define DIR_DIR 2
#define DIR_REG 3
#define REG_REG 4

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
 * @return char* name of the current directory or NULL on error
 */
extern char *getCurDir(void);
/**
 * @brief checks what type of file filename is (regular file, directory, or neither)
 *
 * @param filename name of the file to check its filetype
 * @return int 1 = directory, 2 = regular file, -1 = invalid filetype
 */
extern int checkFiletype(const char *filename);
/**
 * @brief recursively traverses down a directory tree checking for groups of duplicates files
 *
 * @param dirName name of directory being searched in
 * @param parentDir name of directory search started in
 * @param groupsList list of Group that hold information about duplicate files
 * @return int 1 = success, -1 = error
 */
extern int traverseDir(const char *dir, const char *parentDir, Groups *groupsList);
/**
 * @brief checks if a file is already marked as duplicate in a Group
 *
 * @param groupsList list of Group that hold information about duplicate files
 * @param pathname pathname to file being checked
 * @return int 1 = is already marked as a duplicate, 0 = not marked as a duplicate
 */
int isAlreadyDup(Groups *groupsList, const char *pathname);
/**
 * @brief creates a new group of identical files
 *
 * @param groupsCount total number of groups of identical files found so far
 * @param pathname pathname of a file for which an identical file was found
 * @return Group* new group of identical files
 */
Group *setupGroup(int groupsCount, const char *pathname);
/**
 * @brief updates a group of identical files by adding 1 new identical file
 *
 * @param group group that the new identical file will belong to
 * @param pathname pathname of the new identical file
 */
void updateGroup(Group *group, const char *pathname);
extern void compareDirs(void);
/**
 * @brief checks a file for any duplicates recursively through a directory tree
 *
 * @param fileInfo data related to file
 * @param filePathname pathname to file
 * @param dirName directory to recursively search for duplicate files
 * @param groupsList list of Group that hold information about duplicate files
 * @return int 1 = success, -1 = error
 */
extern int compareDirReg(struct stat fileInfo, const char *filePathname, const char *dirName, Groups *groupsList);
extern int compareRegs(char *pathname1, char *pathname2, Groups *groupsList);
extern char *getBasename(char *filename, char *pathname);
void fullRegsCheck(const char *filePathname, struct stat fileInfo, const char *entryPathname, struct stat entryInfo, Groups *groupsList);
/**
 * @brief opens and reads two files for comparison and returns if they are exactly identical
 *
 * @param pathname1 pathname of the file to compare to pathname2
 * @param pathname2 pathname of the file to compare to pathname1
 * @return int 1 = files identical, 0 = files are different, -1 = error
 */
int checkRegs(const char *pathname1, const char *pathname2);

#endif
