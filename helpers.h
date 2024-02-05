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
extern void printDups(void);

#endif
