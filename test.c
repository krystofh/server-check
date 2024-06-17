#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>

// Global variables
static volatile sig_atomic_t executing = 1;

static void handle_exit(int _)
{
    printf("Terminating server check.\n");
    (void)_;
    executing = 0;
}

int check_availability(char *ip)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ping %s -i 0.5 -c 2 -W 1 2>&1 >/dev/null", ip); // ping, wait (-W) 1 s at max., surrpress terminal output
    int exit_status = system(cmd);                                              // execute a ping command and check its exit code
    return exit_status;
}

static inline void get_status(int exit_code)
{
    switch (exit_code)
    {
    case 0:
        printf("OK");
        break;
    case 256:
        printf("UNREACHABLE");
        break;
    default:
        printf("UNKNOWN"); // or some other message
        break;
    }
}

int main(int argc, char *argv[])
{
    signal(SIGINT, handle_exit);
    signal(SIGTSTP, handle_exit);

    if (argc < 2)
    { // check number of arguments
        printf("Error! Please provide at least one IP to check.");
        return 1;
    }
    else
    {
        char **targets = (char **)malloc((argc - 1) * sizeof(char *)); // allocate memory to the array of IPs
        for (int i = 1; i < argc; i++)
        {
            targets[i - 1] = argv[i]; // parse arguments from the terminal
        }

        // Checking servers in a loop
        int counter = 0;

        printf("Checking servers...\n");
        while (executing)
        {
            printf("%d", counter);
            fflush(stdout);
            for (int i = 0; i < argc - 1; i++)
            {
                printf(", ");
                // Ping the IPs
                get_status(check_availability(targets[i]));
                fflush(stdout);
            }
            printf("\n");
            fflush(stdout);
            counter++;
        }
        free(targets);
        // signal(SIGINT, SIG_DFL);
    }
    return 0;
}
