/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "traverse.h"
#include "helpers.h"

int main(int argc, char *argv[])
{
    // could not find the max size for a pathname
    if (PATHNAME_MAX == -1)
        printErr();

    Groups groupsList = {0, (Group **)malloc(10 * sizeof(Group *))};

    // only traverse the current directory
    if (argc == 1)
        traverseDir(".", ".", &groupsList);
    else
    {
        // loop through every file/directory given in the command line
        for (int i = 1; i < argc; i++)
        {
            switch (checkFiletype(argv[i]))
            {
            case 1: // directory
                // TODO: compare directories to each other
                traverseDir(argv[i], argv[i], &groupsList);
                break;
            case 2: // regular file
                /* TODO
                    1. if other command line arguments are regular files:
                        compare the given file to all other files specified in the command line
                    2. if other command line arguments are directories:
                        traverse the given directory and compare all files there to the given file
                */
                // struct stat fileInfo;
                // stat(argv[i], &fileInfo);
                // checkDupsFile(basename(argv[i]), fileInfo, argv[i], ".", &groupsList);
                break;
            default: // invalid filetype
                break;
            }
        }
    }

    printDups(&groupsList);
    freeDups(&groupsList);

    return 0;
}
