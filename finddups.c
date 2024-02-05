/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "traverse.h"
#include "helpers.h"

int main(int argc, char const *argv[])
{
    // could not find the max size for a pathname
    if (PATHNAME_MAX == -1)
        printErr();

    Group *groups = malloc(10 * sizeof(Group));

    // only traverse the current directory
    if (argc == 1)
    {
        char *dirName = getCurDir(); // pathname to a directory
        traverseDir(dirName, dirName, groups);
        free(dirName);
    }
    else
    {
        // loop through every file/directory given in the command line
        for (int i = 1; i < argc; i++)
        {
        }
    }

    printDups();
    free(groups);

    return 0;
}
