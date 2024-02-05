/**
 *  Alex Carbajal
 *  CPT_S 360, Spring 2024
 *  finddups
 */

#include "traverse.h"
#include "helpers.h"

char *getCurDir(void)
{
    char *dirName = malloc(PATHNAME_MAX);
    // invalid pathname
    if (getcwd(dirName, PATHNAME_MAX) == NULL)
    {
        free(dirName);
        printErr();
    }

    return dirName;
}

void traverseDir(char *dirName, char *parentDir, Group *groups)
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
                // printf("%s is a directory\n", entry->d_name);
                traverseDir(newPathname, parentDir, groups); // recursive call
            }
            break;
        case __S_IFREG: // regular file
            // printf("%s is a regular file\n", entry->d_name);
            checkDupsFile(entry, entryInfo, newPathname, parentDir, groups);
            break;
        default: // otherwise file is ignored
            break;
        }
    }

    closedir(dir);
}

void checkDupsFile(struct dirent *file, struct stat fileInfo, char *filePathname, char *dirName, Group *groups)
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
        stat(newPathname, &entryInfo);

        switch (entryInfo.st_mode & __S_IFMT)
        {
        case __S_IFDIR:
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0)
                checkDupsFile(file, fileInfo, filePathname, newPathname, groups); // recursive call
            break;
        case __S_IFREG: // compare the files
            /*
                don't compare the file to itself
                files should be the same size in bytes if they are to be duplicates
                each file should have the exact same byte data
            */
            if (strcmp(entry->d_name, file->d_name) != 0 &&
                entryInfo.st_size == fileInfo.st_size &&
                compareFiles(filePathname, newPathname))
            {
                printf("%s and %s are duplicates\n", file->d_name, entry->d_name);
            }
            break;
        default:
            break;
        }
    }
}

int compareFiles(char *pathname1, char *pathname2)
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