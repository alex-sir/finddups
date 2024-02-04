/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "helpers.h"

void printErr(void)
{
    printf("%s\n", strerror(errno));
    exit(EXIT_FAILURE);
}

void printDirContents(DIR *dir)
{
    struct dirent *entry;
    for (int i = 1; (entry = readdir(dir)); i++)
        printf("%d %s\n", i, entry->d_name);
        
    rewinddir(dir);
}

void printDups(void)
{
}
