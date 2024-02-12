/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#ifndef TRAVERSE
#define TRAVERSE

#include <stdio.h>    // for print()
#include <stdlib.h>   // for exit()
#include <string.h>   // for strerror(), strcmp()
#include <limits.h>   // for _PC_PATH_MAX
#include <errno.h>    // for errno
#include <dirent.h>   // for opendir(), readdir()
#include <unistd.h>   // for stat()
#include <sys/stat.h> // for struct stat

#define PATHNAME_MAX pathconf(".", _PC_PATH_MAX) + 1 // max number of bytes in a file pathname
#define FILE_DIR 1
#define FILE_REG 2
#define FILE_INV 0
#define DIR_DIR 2
#define DIR_REG 3
#define REG_REG 4
#define GROUPS_NUM_GROUP 100 // initial number of groups allocated to Groups and added if full
#define GROUP_NUM_PM 50      // initial number of pathnames allocated to a group and added if full

// group of duplicate files
typedef struct
{
    int id;
    int count;
    char **pathnames;
} Group;

// group of Group
typedef struct
{
    int count;
    Group **members;
} Groups;

/**
 * @brief checks what type of file filename is (regular file, directory, or neither)
 * @param filename name of the file to check its filetype
 * @return int 1 = directory, 2 = regular file, 0 = invalid filetype, -1 = error
 */
extern int checkFiletype(const char *filename);
/**
 * @brief recursively traverses down a directory tree checking for groups of duplicates files within another directory
 * @param dirName1 name of directory being searched for duplicate files
 * @param dirName2 name of directory files from dirName1 will be compared to
 * @param groupsList list of Group that hold information about duplicate files
 * @return int 1 = success, -1 = error
 */
extern int compareDirDir(const char *dirName1, const char *dirName2, Groups *groupsList);
/**
 * @brief checks a file for any duplicates recursively through a directory tree
 * @param fileInfo data related to file
 * @param filePathname pathname to file
 * @param dirName directory to recursively search for duplicate files
 * @param groupsList list of Group that hold information about duplicate files
 * @return int 1 = success, -1 = error
 */
extern int compareDirReg(struct stat fileInfo, const char *filePathname, const char *dirName, Groups *groupsList);
/**
 * @brief compares two regular files to see if they are duplicates
 * @param pathname1 pathname to first file
 * @param pathname2 pathname to second file
 * @param groupsList list of Group that hold information about duplicate files
 * @return int 1 = success, -1 = error
 */
extern int compareRegReg(char *pathname1, char *pathname2, Groups *groupsList);
/**
 * @brief checks if a file is already marked as duplicate in a Group by checking its inode number
 * @param groupsList list of Group that hold information about duplicate files
 * @param pathname pathname to file being checked
 * @param fileInfo stat struct that contains information about the file being checked
 * @return int 1 = is already marked as a duplicate, 0 = not marked as a duplicate
 */
extern int isAlreadyDup(Groups *groupsList, const char *pathname, struct stat fileInfo);
/**
 * @brief creates a new group of identical files
 * @param groupsList list of Group that hold information about duplicate files
 * @param pathname pathname of a file for which an identical file was found
 */
extern void setupGroup(Groups *groupsList, const char *pathname);
/**
 * @brief updates a group of identical files by adding 1 new found identical file
 * @param group group that the new identical file will belong to
 * @param pathname pathname of the new identical file
 * @param groupsList list of Group that hold information about duplicate files
 */
extern void updateGroup(Group *group, const char *pathname, Groups *groupsList);
/**
 * @brief performs a comprehensive duplicity comparison between two given files
 * @param filePathname pathname to a file
 * @param fileInfo stat struct that contains information about the file located at filePathname
 * @param entryPathname pathname to an entry that will be compared with filePathname
 * @param entryInfo stat struct that contains information about the file located at entryPathname
 * @param groupsList list of Group that hold information about duplicate files
 */
extern void fullRegsCheck(const char *filePathname, struct stat fileInfo, const char *entryPathname, struct stat entryInfo, Groups *groupsList);
/**
 * @brief opens and reads two files for comparison and returns if they are exactly identical
 * @param pathname1 pathname of the file to compare to pathname2
 * @param pathname2 pathname of the file to compare to pathname1
 * @return int 1 = files identical, 0 = files are different, -1 = error
 */
extern int checkRegs(const char *pathname1, const char *pathname2);

#endif
