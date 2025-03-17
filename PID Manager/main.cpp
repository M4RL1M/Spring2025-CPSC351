// main.cpp
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include "manager.h"

int main() {
    // Create a child process using fork()
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    }
    else if (pid == 0) {
        // Child process
        // Allocate a bitmap for the PIDs
        PidManager ChildManager;
        if (ChildManager.allocate_map() == -1) {
            std::cerr << "Failed to allocate bitmap." << std::endl;
            return 1;
        }

        // Allocate a PID for the Child Process
        int child_pid = ChildManager.allocate_pid();
        if (child_pid == -1) {
            std::cerr << "Failed to allocate PID for child process." << std::endl;
            return 1;
        }
        std::cout << "Child PID: " << child_pid << std::endl;

        // Release the child PID when done
        ChildManager.release_pid(child_pid);
        std::cout << "Child PID released" << std::endl;
    }
    else {
        // Parent process
        // Wait for Child Process
        // wait(NULL);
        PidManager ParentManager;
        // Allocate a bitmap for PIDs
        if (ParentManager.allocate_map() == -1) {
            std::cerr << "Failed to allocate bitmap." << std::endl;
            return 1;
        }

        // Allocate a PID for the Parent Process
        int parent_pid = ParentManager.allocate_pid();
        if (parent_pid == -1) {
            std::cerr << "Failed to allocate PID for parent process." << std::endl;
            return 1;
        }
        std::cout << "Parent PID: " << parent_pid << std::endl;

        // Release the parent PID when done
        ParentManager.release_pid(parent_pid);
        std::cout << "Parent PID released" << std::endl;

    }

    return 0;
}
