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
        stat(newPathname, &entryInfo);

        // check if the file is a directory or a regular file
        switch (entryInfo.st_mode & __S_IFMT)
        {
        case __S_IFDIR: // directory
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
            {
                traverseDir(newPathname, parentDir, groupsList); // recursive call
            }
            break;
        case __S_IFREG: // regular file
            if (!isAlreadyDup(groupsList, newPathname))
                checkDupsFile(entry, entryInfo, newPathname, parentDir, groupsList);
            break;
        default: // otherwise file is ignored
            break;
        }
    }

    closedir(dir);
}

int isAlreadyDup(Groups *groupsList, char *pathname)
{
    Group *curGroup;
    int isDup = 0;

    for (int i = 0; i < groupsList->count && !isDup; i++)
    {
        curGroup = groupsList->members[i];

        for (int j = 0; j < curGroup->count; j++)
        {
            if (strcmp(curGroup->pathnames[j], pathname) == 0)
            {
                isDup = 1;
                break;
            }
        }
    }

    return isDup;
}

void checkDupsFile(struct dirent *file, struct stat fileInfo, const char *filePathname, const char *dirName, Groups *groupsList)
{
    int foundGroup = 0;
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
        stat(newPathname, &entryInfo);

        switch (entryInfo.st_mode & __S_IFMT)
        {
        case __S_IFDIR:
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                checkDupsFile(file, fileInfo, filePathname, newPathname, groupsList); // recursive call
            break;
        case __S_IFREG: // compare the files
            /*
                don't compare the file to itself
                files should be the same size in bytes if they are to be duplicates
                each file should have the exact same byte data
            */
            if (strcmp(entry->d_name, file->d_name) != 0 &&
                entryInfo.st_size == fileInfo.st_size &&
                compareFiles(filePathname, newPathname) &&
                !isAlreadyDup(groupsList, entry->d_name))
            {
                if (!foundGroup)
                {
                    foundGroup = 1;
                    groupsList->count++;
                    groupsList->members[groupsList->count - 1] = setupGroup(groupsList->count, file->d_name);
                    updateGroup(groupsList->members[groupsList->count - 1], entry->d_name);
                }
                else
                    updateGroup(groupsList->members[groupsList->count - 1], entry->d_name);
            }
            break;
        default:
            break;
        }
    }
}

Group *setupGroup(int groupsCount, char *pathname)
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

void updateGroup(Group *group, char *pathname)
{
    group->count++;
    group->pathnames[group->count - 1] = (char *)malloc(strlen(pathname) + 1);
    strcpy(group->pathnames[group->count - 1], pathname);
}

int compareFiles(const char *pathname1, const char *pathname2)
{
    /*
        moe, larry, and curly are dups
        shemp is not a dup
    */
    FILE *f1 = fopen(pathname1, "r");
    // file could not be opened for reading, stop the program
    if (f1 == NULL)
    {
        printf("%s: %s\n", pathname1, strerror(errno));
        return 0;
    }

    FILE *f2 = fopen(pathname2, "r");
    if (f2 == NULL)
    {
        printf("%s: %s\n", pathname2, strerror(errno));
        return 0;
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