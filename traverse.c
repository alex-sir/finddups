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
        return NULL;
    }

    return dirName;
}

int checkFiletype(const char *filename)
{
    struct stat fileInfo;
    stat(filename, &fileInfo);

    switch (fileInfo.st_mode & __S_IFMT)
    {
    case __S_IFDIR:
        return FILE_DIR;
        break;
    case __S_IFREG:
        return FILE_REG;
        break;
    default:
        return FILE_INV;
        break;
    }
}

int traverseDir(const char *dirName, const char *parentDir, Groups *groupsList)
{
    DIR *dir; // the directory
    dir = opendir(dirName);
    // directory stream could not be opened
    if (dir == NULL)
    {
        printErr();
        return -1;
    }

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
                compareDirReg(entryInfo, newPathname, parentDir, groupsList);
            break;
        default: // otherwise file is ignored
            break;
        }
    }

    closedir(dir);
    return 1;
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

void compareDirs(void)
{
}

int compareDirReg(struct stat fileInfo, const char *filePathname, const char *dirName, Groups *groupsList)
{
    DIR *dir;
    dir = opendir(dirName);
    if (dir == NULL)
    {
        printErr();
        return -1;
    }

    struct dirent *entry;
    struct stat entryInfo;

    while ((entry = readdir(dir)))
    {
        char entryPathname[PATHNAME_MAX];
        sprintf(entryPathname, "%s/%s", dirName, entry->d_name);
        if (strncmp(entryPathname, "./", 2) == 0)
            memmove(entryPathname, entryPathname + 2, strlen(entryPathname) - 1);
        stat(entryPathname, &entryInfo);

        switch (entryInfo.st_mode & __S_IFMT)
        {
        case __S_IFDIR:
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                compareDirReg(fileInfo, filePathname, entryPathname, groupsList); // recursive call
            break;
        case __S_IFREG: // compare the files
            fullRegsCheck(filePathname, fileInfo, entryPathname, entryInfo, groupsList);
            break;
        default:
            break;
        }
    }

    closedir(dir);
    return 1;
}

int compareRegs(char *pathname1, char *pathname2, Groups *groupsList)
{
    struct stat fileInfo1, fileInfo2;
    if (stat(pathname1, &fileInfo1) == -1)
    {
        printErr();
        return -1;
    }
    if (stat(pathname2, &fileInfo2) == -1)
    {
        printErr();
        return -1;
    }
    fullRegsCheck(pathname1, fileInfo1, pathname2, fileInfo2, groupsList);

    return 0;
}

char *getBasename(char *filename, char *pathname)
{
    char *pathnameBasename = "";

    if ((pathnameBasename = strrchr(pathname, '/') + 1))
        strcpy(filename, pathnameBasename);
    else
        strcpy(filename, pathname);

    return filename;
}

void fullRegsCheck(const char *filePathname, struct stat fileInfo, const char *entryPathname, struct stat entryInfo, Groups *groupsList)
{
    /*
        files should be the same size in bytes if they are to be duplicates
        don't compare the file to itself
        each file should have the exact same byte data
    */
    if (fileInfo.st_size == entryInfo.st_size &&
        strcmp(filePathname, entryPathname) != 0 &&
        checkRegs(filePathname, entryPathname))
    {
        int isFileDup = isAlreadyDup(groupsList, filePathname),
            isEntryDup = isAlreadyDup(groupsList, entryPathname);
        // if both not dups, make a new group and add file and entry to it
        if (!isFileDup && !isEntryDup)
        {
            groupsList->count++;
            groupsList->members[groupsList->count - 1] = setupGroup(groupsList->count, filePathname);
            updateGroup(groupsList->members[groupsList->count - 1], entryPathname);
        }
        else if (!isFileDup)
            updateGroup(groupsList->members[groupsList->count - 1], filePathname);
        else if (!isEntryDup)
            updateGroup(groupsList->members[groupsList->count - 1], entryPathname);
    }
}

int checkRegs(const char *pathname1, const char *pathname2)
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
