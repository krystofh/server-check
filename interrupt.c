#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static volatile sig_atomic_t keep_running = 1;

static void sig_handler(int _)
{
    (void)_;
    keep_running = 0;
}

int check_availability(char *ip)
{
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "ping %s -i 0.5 -c 2 -W 1 2>&1 >/dev/null", ip); // ping, wait (-W) 1 s at max., surrpress terminal output
    int exit_status = system(cmd);                                              // execute a ping command and check its exit code
    return exit_status;
}

int main(void)
{
    signal(SIGINT, sig_handler);
    int counter = 0;
    while (keep_running)
    {
        printf("%d \n", counter);
        check_availability("192.168.107.1");
        sleep(1);
        counter++;
    }

    puts("Stopped by signal `SIGINT'");
    return EXIT_SUCCESS;
}