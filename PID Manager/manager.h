// PidManager.h
#ifndef MANAGER_H
#define MANAGER_H

#include <bitset>
#include <iostream>

#define MIN_PID 100
#define MAX_PID 1000

class PidManager {
private:
    std::bitset<MAX_PID - MIN_PID + 1> pid_bitmap;
    bool map_initialization = false;

public:
    // Constructor: accept a minimum PID value to support parent/child independence
    PidManager();

    // Creates and initializes a data structure for representing pids; returns −1 if unsuccessful, 1 if successful
    int allocate_map();

    // Allocates a PID (returns -1 if unable to allocate a PID)
    int allocate_pid();

    // Releases a PID
    void release_pid(int pid);
};

#endif  // MANAGER_H
