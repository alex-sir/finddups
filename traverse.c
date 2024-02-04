/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "traverse.h"
#include "helpers.h"

DIR *getDir(const long PATHNAME_MAX)
{
    char curDir[PATHNAME_MAX]; // pathname to a directory
    // invalid pathname
    if (getcwd(curDir, PATHNAME_MAX) == NULL)
        printErr();

    DIR *dir; // the directory
    dir = opendir(curDir);
    // directory stream could not be opened
    if (dir == NULL)
        printErr();

    return dir;
}

void traverseDir(DIR *dir, Group *groups)
{
    struct dirent *entry; // an entry in a directory
    struct stat entryInfo;

    while ((entry = readdir(dir)))
    {
        stat(entry->d_name, &entryInfo);
        // directory and regular file
        switch (entryInfo.st_mode & __S_IFMT)
        {
        case __S_IFDIR: // directory
            printf("%s is a directory\n", entry->d_name);
            /* recursive call?
                DIR *childDir; // child directory
                childDir = opendir(entry->d_name);
                traverseDir(childDir, groups);
            */
            break;
        case __S_IFREG: // regular file
            printf("%s is a regular file\n", entry->d_name);
            break;
        }
    }
}
