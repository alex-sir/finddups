/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "traverse.h"
#include "helpers.h"

int main(int argc, char const *argv[])
{
    Group *groups = malloc(10 * sizeof(Group));
    const long PATHNAME_MAX = pathconf(".", _PC_PATH_MAX);
    // could not find the max size for a pathname
    if (PATHNAME_MAX == -1)
        printErr();
    DIR *dir; // the directory

    // only traverse the current directory
    if (argc == 1)
    {
        dir = getDir(PATHNAME_MAX);
        traverseDir(dir, groups);
    }
    else
    {
        // loop through every file/directory given in the command line
        for (int i = 1; i < argc; i++)
        {
        }
    }

    closedir(dir);
    printDups();
    free(groups);

    return 0;
}
