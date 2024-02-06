/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#ifndef HELPERS
#define HELPERS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <dirent.h>

#include "traverse.h"

/**
 * @brief prints an error message and exits the program
 */
extern void printErr(void);
/**
 * @brief prints all the contents in a directory (non-recursive)
 *
 * @param dir directory to have its contents printed
 */
extern void printDirContents(DIR *dir);
/**
 * @brief prints out all duplicate file groups with n members found in a directory tree from
 *        a starting directory in form "n 1 pathOfFirstFile | n 2 pathOfSecondFile ..."
 *
 * @param groupsList list of groups containing all duplicate file groups
 */
extern void printDups(Groups *groupsList);
/**
 * @brief frees all memory space related to a Groups struct
 *
 * @param groupsList Groups list to have all its memory freed
 */
extern void freeDups(Groups *groupsList);

#endif
