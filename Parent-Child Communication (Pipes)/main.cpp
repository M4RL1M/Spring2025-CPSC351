#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cstring>
#include <string>
#include "manager.h"

#define BUFFER_SIZE 25
#define READ_END 0
#define WRITE_END 1

int main() {
    char write_msg[BUFFER_SIZE] = "Greetings";
    char read_msg[BUFFER_SIZE];
    
    // create the pipe
    int fd[2];
    if (pipe(fd) == -1) {
        std::cerr << "Pipe failed" << std::endl;
        return 1;
    }

    // fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        std::cerr << "Fork failed!" << std::endl;
        return 1;
    } else if (pid > 0) {
        // Parent process
        PidManager ParentManager;
        if (ParentManager.allocate_map() == -1) {
            std::cerr << "Failed to allocate bitmap." << std::endl;
            return 1;
        }
    
        int parent_pid = ParentManager.allocate_pid();
        if (parent_pid == -1) {
            std::cerr << "Failed to allocate PID for parent process." << std::endl;
            return 1;
        }
        std::cout << "Parent PID: " << parent_pid << std::endl;
    
        while (true) {
            // Wait for Child Process to issue allocation requests or send end signal
            wait(NULL);
    
            read(fd[READ_END], read_msg, BUFFER_SIZE);
            if (strcmp(read_msg, "DONE") == 0) break;
    
            // Allocate Child PID
            if (strcmp(read_msg, "ALLOCATE") == 0) {
                int child_pid = ParentManager.allocate_pid();
                write(fd[WRITE_END], &child_pid, sizeof(child_pid));
                std::cout << "Parent allocated PID: " << child_pid << std::endl;
            }
            
            // wait(NULL);

            // Release Child PID
            read(fd[READ_END], read_msg, BUFFER_SIZE);
            if (strcmp(read_msg, "RELEASE") == 0) {
                int child_pid;
                read(fd[READ_END], &child_pid, sizeof(child_pid));
                ParentManager.release_pid(child_pid);
                std::cout << "Parent released PID: " << child_pid << std::endl;
            }
        }
    
        close(fd[READ_END]);
        close(fd[WRITE_END]);
    
    } else {
        // Child process
        PidManager ChildManager;
        if (ChildManager.allocate_map() == -1) {
            std::cerr << "Failed to allocate bitmap." << std::endl;
            return 1;
        }
    
        int child_pid = ChildManager.allocate_pid();
        if (child_pid == -1) {
            std::cerr << "Failed to allocate PID for child process." << std::endl;
            return 1;
        }
        std::cout << "Child PID: " << child_pid << std::endl;
    
        int iterations = 5;
        for (int i = 0; i < iterations; i++) {
            // Send "ALLOCATE" request
            char allocate[] = "ALLOCATE";
            write(fd[WRITE_END], allocate, strlen(allocate) + 1);
    
            // Wait for the parent to allocate and return the PID
            // wait(NULL);
            int received_pid;
            read(fd[READ_END], &received_pid, sizeof(received_pid));
            std::cout << "Child received allocated PID: " << received_pid << std::endl;
    
            // Send "RELEASE" request
            char release[] = "RELEASE";
            write(fd[WRITE_END], release, strlen(release) + 1);
            // wait(NULL);
            write(fd[WRITE_END], &received_pid, sizeof(received_pid));
    
            // Wait for the parent to release the PID
            wait(NULL);
    
            if (i == (iterations - 1)) {
                char done[] = "DONE";
                write(fd[WRITE_END], done, strlen(done) + 1);
            }
        }
    
        close(fd[READ_END]);
        close(fd[WRITE_END]);
    }
    
    return 0;
}