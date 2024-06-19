/*findsym /tmp /tmp/readme.txt
/tmp/README -> readme.txt
/tmp/test/ctime -> ../readme.txt
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h> // for basename cmd

// Sources
// [1] https://linux.die.net/man/1/find
// [2] https://linux.die.net/man/3/basename

#define DEBUG 0 // activate debug console output

int main(int argc, char *argv[])
{
    // Parse arguments
    if (argc < 3)
    {
        fprintf(stderr, "Usage: findsym <PARENT_FOLDER> <FILE>\nlook for symbolic link to <FILE> in <PARENT_FOLDER>");
        return 1;
    }
    char *parent_dir = argv[1];
    char *file = basename(argv[2]);

#if DEBUG
    printf("Looking for file: %s\n", file);
#endif

    // Look for links
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "find %s/ -lname \"%s\" -ls", parent_dir, file);
    // Note: alternative find PARENT_FOLDER -type l -ls | grep FILE and using pipe
#if DEBUG
    printf("Executing command: %s\n", cmd);
#endif
    int status = system(cmd);
    return 0;
}