# Finddups

Scans a set of files looking for duplicates it will organize into groups. This only works with directories (dir(s)) and regular files (reg(s)).

## How It Works

If a directory is specified, the directory will recursively compare all files within itself before being compared with something else. A regular file by itself has nothing to be compared to, thus no operation is performed.

All arguments specified in the command line will be compared against one another in a sequential manner:

``` bash
./finddups dir1 reg2 reg3 dir4
```

* Compare dir1 to reg2, then reg3, then dir4
* Compare reg2 to reg3, then dir4
* Compare reg3 to dir4
* Done

There are 3 cases for comparison:

1. Compare directories to directories:
   * Algorithm:
     * Get one file from the first directory and compare it to all files in the other directory
     * Repeat this for all files in the first directory

2. Compare directories to regular files:
    * Algorithm: Compare the regular file to all files in the directory

3. Compare regular files to regular files:
   * Algorithm: A simple direct byte-by-byte comparison of both files
