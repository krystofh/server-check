/**
   @file server-check.c
   @author Krystof Hes
   @brief Testing availability of one or multiple servers using ping command executed in parallel.
   @version 1.0
   @date 2024-06-18

   @copyright Copyright (c) 2024

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

// ... Sources ...
// [1] Unix Threads in C
// https://www.youtube.com/watch?v=d9s_d28yJq0&list=PLfqABt5AS4FmuQf70psXrsMLEDQXNkLq2
// [2] https://www.geeksforgeeks.org/multithreading-in-c/
// [3] certain level of support from ChatGPT

#define DEBUG 0 // extended console log for debugging

// Print server status based on the exit code of ping command
void print_status(int exit_code)
{
    switch (exit_code)
    {
    case 0:
        printf(", OK");
        break;
    case 256:
        printf(", UNREACHABLE");
        break;
    default:
        printf(",UNKNOWN"); // or some other message
        break;
    }
}

void *check_ip(void *arg)
{
    const char *ip = (const char *)arg; // pointer to IP address of target
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ping %s -c 1 -W 1 2>&1 >/dev/null", ip); // ping, wait (-W) 1 s at max., surrpress terminal output
    // execute a ping command and check its exit code
    int status = system(cmd);
    print_status(status);

    int *exit_code = malloc(sizeof(int));
    *exit_code = status;
    return exit_code;
}

int main(int argc, char *argv[])
{
    // signal(SIGINT, handle_exit);
    int n_servers = argc - 1; // number of IPs provided
    if (!n_servers)
    { // check number of arguments
        printf("Error! Please provide at least one IP to check.");
        return 1;
    }
    else
    {
        // Parse IP addresses
        char **targets = (char **)malloc((n_servers) * sizeof(char *)); // allocate memory to the array of IPs
        for (int i = 0; i < n_servers; i++)
        {
            targets[i] = argv[i + 1]; // parse arguments from the terminal
        }

        // Checking process outputs in a loop
        int counter = 0;
        printf("Checking servers...\n");
        while (1)
        {
            int servers_ok = 1;
            struct timespec start_time, end_time, wait_time;

            // Tick
            clock_gettime(CLOCK_MONOTONIC, &start_time); // Get current time
            // Status
            printf("%d", counter);

            // Create server status checking child processes
            pthread_t threads[n_servers];
            for (int i = 0; i < n_servers; i++)
            {
                if (pthread_create(&threads[i], NULL, check_ip, (void *)targets[i]) != 0)
                {
                    fprintf(stderr, "Error creating thread\n");
                    return 1;
                }
            }
            // Processes termination
            for (int i = 0; i < n_servers; i++)
            {
                void *thread_ret_val;                      // declare a generic pointer for storing thread exit code
                pthread_join(threads[i], &thread_ret_val); // wait for child processes, save exit code
                int *exit_code = (int *)thread_ret_val;    // cast to integer pointer

                if (*exit_code != 0) // if a single IP was not reachable, set flag
                {
                    servers_ok = 0;
                }
                free(exit_code);
            }
            // Calculate run time in the current period
            clock_gettime(CLOCK_MONOTONIC, &end_time); // Get current time
            long s = end_time.tv_sec - start_time.tv_sec;
            long ns = end_time.tv_nsec - start_time.tv_nsec;
            double runtime_s = s + ns * 1e-9;
            // Wait for the rest of the period
            wait_time.tv_sec = 0;
            if (s >= 1)
            {
                wait_time.tv_nsec = 0; // no sleep
            }
            else
            {
                wait_time.tv_nsec = 1 * 1e9 - ns; // sleep for 1s - runtime in ns
            }
#if DEBUG
            printf(" run: %.3f wait: %.3f", runtime_s, wait_time.tv_nsec * 1e-9);
#endif
            printf("\n");
            fflush(stdout);
            nanosleep(&wait_time, NULL);
            counter++;
        }
        free(targets);
    }
    return EXIT_SUCCESS;
}
