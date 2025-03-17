#include <iostream>
#include <cstdlib>
#include <ctime>
#include "manager.h"

// Constants for the PID range
#define MIN_PID 100
#define MAX_PID 1000

// Test error handling by calling allocate_pid before calling allocate_map
void test_allocate_pid_before_allocate_map() {
    PidManager pid_manager;
    int pid = pid_manager.allocate_pid();
    if (pid == -1) {
        std::cout << "Test Passed: allocate_pid returned -1 before calling allocate_map." << std::endl;
    } else {
        std::cerr << "Test Failed: allocate_pid failed to return -1 before calling allocate_map." << std::endl;
    }
}

// Verify that allocate_pid returns -1 if allocate_map was not called
void test_allocate_pid_no_initialize() {
    PidManager pid_manager;
    int pid = pid_manager.allocate_pid();
    if (pid == -1) {
        std::cout << "Test Passed: allocate_pid returned -1 when bitmap is not initialized." << std::endl;
    } else {
        std::cerr << "Test Failed: allocate_pid should return -1 when not bitmap is not initialized." << std::endl;
    }
}

// Test releasing a PID without initializing the bitmap
void test_release_pid_without_initialize() {
    PidManager pid_manager;
    pid_manager.release_pid(100);
    std::cout << "Test Passed: Releasing PID without initializing bitmap did not cause error." << std::endl;
}

// Allocate and release PIDs in a loop
void test_allocate_release_loop() {
    PidManager pid_manager;
    if (pid_manager.allocate_map() == -1) {
        std::cerr << "Error initializing the PID map." << std::endl;
        return;
    }

    const int iterations = 1000;
    for (int i = 0; i < iterations; ++i) {
        int pid = pid_manager.allocate_pid();
        if (pid == -1) {
            std::cerr << "Test Failed: Allocation failed during iteration " << i << std::endl;
            return;
        }
        pid_manager.release_pid(pid);  // Release the PID
    }

    std::cout << "Test Passed: Successfully allocated and released " << iterations << " PIDs." << std::endl;
}


// Randomly allocate and release PIDs multiple times
void test_random_allocate_release() {
    PidManager pid_manager;
    if (pid_manager.allocate_map() == -1) {
        std::cerr << "Error initializing the bitmap." << std::endl;
        return;
    }

    const int iterations = 500;
    srand(time(0));  // Initialize random seed

    for (int i = 0; i < iterations; ++i) {
        int action = rand() % 2;
        if (action == 0) {
            int pid = pid_manager.allocate_pid();
            if (pid != -1) {
                // std::cout << "Allocated PID: " << pid << std::endl;
                pid_manager.release_pid(pid);
            }
        } else {
            int random_pid = rand() % (MAX_PID - MIN_PID + 1) + MIN_PID;
            pid_manager.release_pid(random_pid);
            // std::cout << "Released PID: " << random_pid << std::endl;
        }
    }
    std::cout << "Test Passed: Successfully allocated and released PIDs randomly." << std::endl;
}

// Verify that the allocated PIDs are within the specified range
void test_pid_range() {
    PidManager pid_manager;
    if (pid_manager.allocate_map() == -1) {
        std::cerr << "Error initializing the bitmap." << std::endl;
        return;
    }

    const int num_allocations = 100;
    for (int i = 0; i < num_allocations; ++i) {
        int pid = pid_manager.allocate_pid();
        if (pid == -1) {
            std::cerr << "Test Failed: PID allocation failed during iteration " << i << std::endl;
            return;
        }
        if (pid < MIN_PID || pid > MAX_PID) {
            std::cerr << "Test Failed: Allocated PID " << pid << " is out of range." << std::endl;
            return;
        }
    }
    std::cout << "Test Passed: All allocated PIDs are within the specified range." << std::endl;
}

// Ensure released PIDs become available for allocation
void test_pid_reusability() {
    PidManager pid_manager;
    if (pid_manager.allocate_map() == -1) {
        std::cerr << "Error initializing the bitmap." << std::endl;
        return;
    }

    int pid = pid_manager.allocate_pid();
    if (pid == -1) {
        std::cerr << "Test Failed: PID allocation failed." << std::endl;
        return;
    }

    pid_manager.release_pid(pid);
    int new_pid = pid_manager.allocate_pid();
    if (new_pid == -1 || new_pid != pid) {
        std::cerr << "Test Failed: Released PID was not reused." << std::endl;
        return;
    }
    std::cout << "Test Passed: Released PID was successfully reused." << std::endl;
}

// Attempt to allocate a PID when the range is exhausted
void test_pid_exhaustion() {
    PidManager pid_manager;
    if (pid_manager.allocate_map() == -1) {
        std::cerr << "Error initializing the bitmap." << std::endl;
        return;
    }

    for (int i = MIN_PID; i <= MAX_PID; ++i) {
        int pid = pid_manager.allocate_pid();
        if (pid == -1) {
            std::cerr << "Test Failed: PID allocation failed when there should be available PIDs." << std::endl;
            return;
        }
    }

    int pid = pid_manager.allocate_pid();
    if (pid == -1) {
        std::cout << "Test Passed: PID allocation failed after all PIDs were used." << std::endl;
    } else {
        std::cerr << "Test Failed: allocate_pid should return -1 when all PIDs are in use." << std::endl;
    }
}

int main() {
    // Run all the tests
    test_allocate_pid_before_allocate_map();
    test_allocate_pid_no_initialize();
    test_release_pid_without_initialize();
    test_allocate_release_loop();
    test_random_allocate_release();
    test_pid_range();
    test_pid_reusability();
    test_pid_exhaustion();
    
    return 0;
}
