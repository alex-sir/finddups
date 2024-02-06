/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "helpers.h"

void printErr(void)
{
    fprintf(stderr, "Error: %s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

void printDirContents(DIR *dir)
{
    struct dirent *entry;
    for (int i = 1; (entry = readdir(dir)); i++)
        printf("%d %s\n", i, entry->d_name);

    rewinddir(dir);
}

void printDups(Groups *groupsList)
{
    Group *curGroup;

    // get every group
    for (int i = 0; i < groupsList->count; i++)
    {
        curGroup = groupsList->members[i];
        // get every pathname in the group
        for (int j = 0; j < curGroup->count; j++)
            printf("%d %d %s\n", curGroup->count, j + 1, curGroup->pathnames[j]);
    }
}

void freeDups(Groups *groupsList)
{
    // free every Group member in groupsList
    for (int i = 0; i < groupsList->count; i++)
    {
        // free every pathname string in Group
        for (int j = 0; j < groupsList->members[i]->count; j++)
            free(groupsList->members[i]->pathnames[j]);

        free(groupsList->members[i]->pathnames);
        free(groupsList->members[i]);
    }

    free(groupsList->members);
}