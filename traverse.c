/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "traverse.h"
#include "helpers.h"

char *getCurDir(void)
{
    char *dirName = (char *)malloc(PATHNAME_MAX);
    // invalid pathname
    if (getcwd(dirName, PATHNAME_MAX) == NULL)
    {
        free(dirName);
        printErr();
    }

    return dirName;
}

int checkFiletype(const char *filename)
{
    struct stat fileInfo;
    stat(filename, &fileInfo);

    switch (fileInfo.st_mode & __S_IFMT)
    {
    case __S_IFDIR: // directory
        return 1;
        break;
    case __S_IFREG: // regular file
        return 2;
        break;
    default: // invalid filetype
        return -1;
        break;
    }
}

void traverseDir(const char *dirName, const char *parentDir, Groups *groupsList)
{
    DIR *dir; // the directory
    dir = opendir(dirName);
    // directory stream could not be opened
    if (dir == NULL)
        printErr();

    struct dirent *entry; // an entry in a directory
    struct stat entryInfo;

    while ((entry = readdir(dir)))
    {
        char newPathname[PATHNAME_MAX];
        sprintf(newPathname, "%s/%s", dirName, entry->d_name);
        // if directory starts with "./", remove it
        if (strncmp(newPathname, "./", 2) == 0)
            memmove(newPathname, newPathname + 2, strlen(newPathname) - 1);
        stat(newPathname, &entryInfo);

        // check if the file is a directory or a regular file
        switch (entryInfo.st_mode & __S_IFMT)
        {
        case __S_IFDIR: // directory
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                traverseDir(newPathname, parentDir, groupsList); // recursive call
            break;
        case __S_IFREG: // regular file
            if (!isAlreadyDup(groupsList, newPathname))
                checkDupsFile(entry->d_name, entryInfo, newPathname, parentDir, groupsList);
            break;
        default: // otherwise file is ignored
            break;
        }
    }

    closedir(dir);
}

/* 3 cases
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
    Can maybe just use checkDupsFile (or a modified version of it)
*/
void traverseDirs(void)
{
}

int isAlreadyDup(Groups *groupsList, const char *pathname)
{
    Group *curGroup;

    // get every group
    for (int i = 0; i < groupsList->count; i++)
    {
        curGroup = groupsList->members[i];
        // get every pathname in the group
        for (int j = 0; j < curGroup->count; j++)
        {
            if (strcmp(curGroup->pathnames[j], pathname) == 0)
                return 1; // already marked as a duplicate
        }
    }

    return 0; // not marked as a duplicate
}

void checkDupsFile(const char *filename, struct stat fileInfo, const char *filePathname, const char *dirName, Groups *groupsList)
{
    DIR *dir;
    dir = opendir(dirName);
    if (dir == NULL)
        printErr();

    struct dirent *entry;
    struct stat entryInfo;

    while ((entry = readdir(dir)))
    {
        char newPathname[PATHNAME_MAX];
        sprintf(newPathname, "%s/%s", dirName, entry->d_name);
        if (strncmp(newPathname, "./", 2) == 0)
            memmove(newPathname, newPathname + 2, strlen(newPathname) - 1);
        stat(newPathname, &entryInfo);

        switch (entryInfo.st_mode & __S_IFMT)
        {
        case __S_IFDIR:
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                checkDupsFile(filename, fileInfo, filePathname, newPathname, groupsList); // recursive call
            break;
        case __S_IFREG: // compare the files
            /*
                files should be the same size in bytes if they are to be duplicates
                don't compare the file to itself
                if a file is already marked as a duplicate it should not be added to a group again
                each file should have the exact same byte data
            */
            if (entryInfo.st_size == fileInfo.st_size &&
                strcmp(entry->d_name, filename) != 0 &&
                !isAlreadyDup(groupsList, newPathname) &&
                compareFiles(filePathname, newPathname))
            {
                if (!isAlreadyDup(groupsList, filePathname))
                {
                    groupsList->count++;
                    groupsList->members[groupsList->count - 1] = setupGroup(groupsList->count, filePathname);
                    updateGroup(groupsList->members[groupsList->count - 1], newPathname);
                }
                else
                    updateGroup(groupsList->members[groupsList->count - 1], newPathname);
            }
            break;
        default:
            break;
        }
    }

    closedir(dir);
}

Group *setupGroup(int groupsCount, const char *pathname)
{
    Group *newGroup;
    newGroup = (Group *)malloc(sizeof(Group));
    newGroup->id = groupsCount;
    newGroup->count = 1;
    newGroup->pathnames = (char **)malloc(10 * (sizeof(char *)));
    newGroup->pathnames[newGroup->count - 1] = (char *)malloc(strlen(pathname) + 1);
    strcpy(newGroup->pathnames[newGroup->count - 1], pathname);

    return newGroup;
}

void updateGroup(Group *group, const char *pathname)
{
    group->count++;
    group->pathnames[group->count - 1] = (char *)malloc(strlen(pathname) + 1);
    strcpy(group->pathnames[group->count - 1], pathname);
}

int compareFiles(const char *pathname1, const char *pathname2)
{
    FILE *f1 = fopen(pathname1, "r");
    // file could not be opened for reading, stop the program
    if (f1 == NULL)
    {
        printf("%s: %s\n", pathname1, strerror(errno));
        return -1;
    }

    FILE *f2 = fopen(pathname2, "r");
    if (f2 == NULL)
    {
        printf("%s: %s\n", pathname2, strerror(errno));
        return -1;
    }

    int ch0 = getc(f1);
    int ch1 = getc(f2);
    // keep reading chars until the end of both files
    while (ch0 != EOF || ch1 != EOF)
    {
        if (ch0 != ch1)
        {
            fclose(f1);
            fclose(f2);
            return 0; // files differ
        }
        ch0 = getc(f1);
        ch1 = getc(f2);
    }

    fclose(f1);
    fclose(f2);
    return 1; // files identical
}
