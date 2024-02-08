/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "traverse.h"
#include "helpers.h"

int main(int argc, char *argv[])
{
    // CURRENT TEST: ./finddups test/brothers test/curly.txt
    /* TODO: 3 cases for comparison
        1. Compare directories to directories
        2. Compare regular files to regular files
        3. Compare directories to regular files
        Algorithm (1, 2)
            Each directory specified in the command line will be compared to every single other directory
            Example: /dir1 /dir2 /dir3 /dir4
                1. Compare /dir1 to /dir2, then /dir3, then /dir4
                2. Compare /dir2 to dir3, then dir4
                3. Compare /dir3 to /dir4
                4. Done
            Similar algorithm will be done for comparing all files specified in the command line
        Algorithm (3)
            Set the directory as the "parent directory" and compare all files there to the regular file
            Repeat this for all combinations of regular files and directories
            Can maybe just use compareDirReg (or a modified version of it)
        Algorithm(1)
            Get one file from the first directory and compare it to all files in the other directory
            Repeat this for all files in the first directory
    */

    // could not find the max size for a pathname
    if (PATHNAME_MAX == -1)
        printErr();

    Groups groupsList = {0, (Group **)malloc(10 * sizeof(Group *))};

    // only traverse the current directory
    if (argc == 1)
        traverseDir(".", ".", &groupsList);
    else
    {
        int curFiletype = 0, comparingToFiletype = 0;
        struct stat fileInfo;
        // loop through every file/directory given in the command line
        for (int curComparison = 1; curComparison < argc; curComparison++)
        {
            switch ((curFiletype = checkFiletype(argv[curComparison])))
            {
            case FILE_DIR:
                traverseDir(argv[curComparison], argv[curComparison], &groupsList);
                break;
            case FILE_REG:
            default: // invalid filetype
                break;
            }

            // compare the current file/directory to every other one it hasn't been compared to yet
            for (int comparingTo = curComparison + 1; comparingTo < argc; comparingTo++)
            {
                comparingToFiletype = checkFiletype(argv[comparingTo]);
                switch (curFiletype + comparingToFiletype)
                {
                case DIR_DIR:
                    break;
                case DIR_REG: // check for duplicates of a regular file in a directory
                    if (curFiletype == FILE_REG)
                    {
                        stat(argv[curComparison], &fileInfo);
                        compareDirReg(fileInfo, argv[curComparison], argv[comparingTo], &groupsList);
                    }
                    else
                    {
                        stat(argv[comparingTo], &fileInfo);
                        compareDirReg(fileInfo, argv[comparingTo], argv[curComparison], &groupsList);
                    }
                    break;
                case REG_REG:
                    compareRegs(argv[curComparison], argv[comparingTo], &groupsList);
                    break;
                default: // something went wrong
                    break;
                }
            }
        }
    }

    printDups(&groupsList);
    freeDups(&groupsList);

    return 0;
}
