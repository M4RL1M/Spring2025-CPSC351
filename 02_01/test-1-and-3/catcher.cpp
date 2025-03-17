#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>

int main() {
    pid_t pid;

    // Fork a child process
    pid = fork();

    if (pid < 0) { // Error occurred
        std::cerr << "Fork Failed" << std::endl;
        return 1;
    }
    else if (pid == 0) { // Child process
        // std::cout << "Hello World" << std::endl;
        execlp("/bin/ls", "ls", NULL);
    }
    else { // Parent process
        // Parent will wait for the child to complete
        sleep(100);
        wait(NULL);
        std::cout << "Child Complete" << std::endl;
    }

    return 0;
}