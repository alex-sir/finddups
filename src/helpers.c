/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "helpers.h"

void printErr(void)
{
    fprintf(stderr, "%s\n", strerror(errno));
}

void printErrExit(void)
{
    printErr();
    exit(EXIT_FAILURE);
}

void printDirContents(DIR *dir)
{
    struct dirent *entry;
    for (int i = 1; (entry = readdir(dir)); i++)
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            printf("%d %s\n", i, entry->d_name);
    }

    rewinddir(dir);
}

int getGroupMaxDigits(Groups *groupsList)
{
    int maxCount = 0, maxDigits = 0;

    for (int i = 0; i < groupsList->count; i++)
    {
        if (groupsList->members[i]->count > maxCount)
            maxCount = groupsList->members[i]->count;
    }

    // get the highest number of digits found in the count of a group
    while (maxCount != 0)
    {
        maxCount /= 10;
        maxDigits++;
    }

    return maxDigits;
}

void printDups(Groups *groupsList)
{
    Group *curGroup;
    int maxDigits = getGroupMaxDigits(groupsList);

    // get every group
    for (int i = 0; i < groupsList->count; i++)
    {
        curGroup = groupsList->members[i];
        // get every pathname in the group
        for (int j = 0; j < curGroup->count; j++)
            printf("%*d %*d %s\n", maxDigits, curGroup->count, maxDigits, j + 1, curGroup->pathnames[j]);
    }
}

void freeDups(Groups *groupsList)
{
    // free every Group member in groupsList
    if (groupsList->count > 0)
    {
        for (int i = 0; i < groupsList->count; i++)
        {
            if (groupsList->members[i]->count > 0)
            {
                // free every pathname string in Group
                for (int j = 0; j < groupsList->members[i]->count; j++)
                {
                    free(groupsList->members[i]->pathnames[j]);
                    groupsList->members[i]->pathnames[j] = NULL;
                }
            }

            free(groupsList->members[i]->pathnames);
            groupsList->members[i]->pathnames = NULL;
            free(groupsList->members[i]);
            groupsList->members[i] = NULL;
        }
    }

    free(groupsList->members);
    groupsList->members = NULL;
}