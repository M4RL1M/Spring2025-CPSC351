#include <cstdlib>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void)
{
    pid_t p = fork();   // creates child process and stores the returned PID

    if (p != 0)         // executed by parent process
    {   sleep(1);       /* the child process terminates while the parent process sleeps,
                           the child then becomes a zombie process because the returned status
                           of the terminated child process isn't checked via a wait() */

        system("ps -eo pid,ppid,stat,cmd");  // prints a list of processes in your terminal

    }
    else        // executed by child process
    {
        exit(0);        // the child process terminates immediately
    }

    return 0;
}