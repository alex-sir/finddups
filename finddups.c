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

    Groups groupsList = {0, (Group **)malloc(10 * sizeof(Group *))};

    // only traverse the current directory
    if (argc == 1)
    {
        char *dirName;
        dirName = getCurDir(); // pathname to a directory
        traverseDir(dirName, dirName, &groupsList);
        free(dirName);
    }
    else
    {
        // loop through every file/directory given in the command line
        for (int i = 1; i < argc; i++)
        {
            const char *dirName;
            dirName = argv[1];
            traverseDir(dirName, dirName, &groupsList);
        }
    }

    printDups(&groupsList);
    freeDups(&groupsList);
    free(groupsList.members);

    return 0;
}
