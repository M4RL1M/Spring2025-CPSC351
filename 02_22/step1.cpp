#include <iostream>
#include <thread>
const int LIMIT =10000;

int x =1;

void increment() {
    for (int i = 0; i < LIMIT; ++i) {
        x++;
        std::cout << "increment x:" << x << std::endl;
       }
}

void decrement() {
    for (int i = 0; i < LIMIT; ++i) {
      x--;
      std::cout << "decrement x:" << x << std::endl;
     }
}
int main() {
        
    // Create threads
    std::thread thread1(increment);
    std::thread thread2(decrement);

    // Join the threads with the main thread
    thread1.join();
    thread2.join();
     
     std::cout << "Modified x:" << x << std::endl;
   

    return 0;
}