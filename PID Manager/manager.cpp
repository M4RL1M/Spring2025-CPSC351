// PidManager.cpp
#include "manager.h"

// Constructor
PidManager::PidManager() {}

// Initializes the PID bitmap
int PidManager::allocate_map() {
    pid_bitmap.reset();  // All PIDs are available initially
    if (pid_bitmap.any()) {  // If any bit is set, the reset failed
        return -1;
    }
    map_initialization = true;
    return 1;  // Success
}

// Allocates a PID; returns -1 if unable to allocate a PID
int PidManager::allocate_pid() {
    if (!map_initialization) {
        return -1; // Attempt to allocate PID before initializing map
    }
    for (int i = 0; i < (MAX_PID - MIN_PID + 1); i++) {
        if (!pid_bitmap[i]) {
            pid_bitmap.set(i);  // Mark this PID as in use
            return MIN_PID + i; // Return the corresponding PID value
        }
    }
    return -1;  // All PIDs are in use
}

// Releases a PID
void PidManager::release_pid(int pid) {
    if (!map_initialization) {
        return; // Attempt to release before initializing map
    }
    
    if (pid < MIN_PID || pid > MAX_PID) {
        return; // Outside scope
    }
    
    int index = pid - MIN_PID;
    if (pid_bitmap[index]) {
        pid_bitmap.reset(index);
    }
}
