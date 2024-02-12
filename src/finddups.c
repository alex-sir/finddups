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
        printErrExit();

    Groups groupsList = {0, (Group **)malloc(GROUPS_NUM_GROUP * sizeof(Group *))};
    if (groupsList.members == NULL)
        printErrExit();

    // only traverse the current directory
    if (argc == 1)
        compareDirDir(".", ".", &groupsList);
    else
    {
        int curFiletype = 0, comparingToFiletype = 0;
        struct stat fileInfo;
        // loop through every file/directory given in the command line
        for (int curComparison = 1; curComparison < argc; curComparison++)
        {
            switch ((curFiletype = checkFiletype(argv[curComparison])))
            {
            case FILE_DIR: // compare directory to itself
                compareDirDir(argv[curComparison], argv[curComparison], &groupsList);
                break;
            case FILE_REG: // do nothing
                break;
            default: // invalid file
                continue;
                break;
            }

            // compare the current file/directory to every other one it hasn't been compared to yet
            for (int comparingTo = curComparison + 1; comparingTo < argc; comparingTo++)
            {
                comparingToFiletype = checkFiletype(argv[comparingTo]);
                switch (curFiletype + comparingToFiletype)
                {
                case DIR_DIR:
                    compareDirDir(argv[comparingTo], argv[curComparison], &groupsList);
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
                    compareRegReg(argv[curComparison], argv[comparingTo], &groupsList);
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
