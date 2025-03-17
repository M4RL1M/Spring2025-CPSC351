#include <sys/wait.h>
#include <sys/types.h>
#include <iostream>
#include <unistd.h>

int main(){
    
    pid_t pid = fork();

    if (pid > 0) {
        
        wait(NULL);
        std::cout << "Parent ID: " << getpid() << std::endl;
        
    }

    else {  

        std::cout << "Child ID: " << getpid() << std::endl;

    }
}