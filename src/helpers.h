/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#ifndef HELPERS
#define HELPERS

#include "traverse.h"

/**
 * @brief prints an error message
 */
extern void printErr(void);
/**
 * @brief prints an error message with the pathname of a file
 */
extern void printErrFile(const char *pathname);
/**
 * @brief prints an error message by calling printErr() and exits with failure
 */
extern void printErrExit(void);
/**
 * @brief prints all the contents in a directory (non-recursive)
 * @param dir directory to have its contents printed
 */
extern void printDirContents(DIR *dir);
/**
 * @brief calculates the highest number of digits found in the count of a group
 * @param groupsList list of Group that hold information about duplicate files
 * @return int highest number of digits found in a count of a group
 */
extern int getGroupMaxDigits(Groups *groupsList);
/**
 * @brief prints out all duplicate file groups with n members found in a directory tree from
 *        a starting directory in form "n 1 pathOfFirstFile | n 2 pathOfSecondFile ..."
 * @param groupsList list of groups containing all duplicate file groups
 */
extern void printDups(Groups *groupsList);
/**
 * @brief frees all memory space related to a Groups struct
 * @param groupsList Groups list to have all its memory freed
 */
extern void freeDups(Groups *groupsList);

#endif
