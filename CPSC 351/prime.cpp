#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include <algorithm>

// Function to check if a number is prime
bool isPrime(int num) {
    // Numbers less than 2 are not prime
    if (num < 2) {
        return false;
    }
    // Check divisibility up to the square root of the number
    // If divisible, it is not a prime
    for (int i = 2; i <= std::sqrt(num); ++i) {
        if (num % i == 0) {
            return false;
        }
    }
    return true;
}

// Thread function to compute prime numbers in a given range
void findPrimes(int start, int end, std::vector<int>& localPrimes) {
    for (int i = start; i <= end; ++i) {
        // Check numbers to see if they are prime
        if (isPrime(i)) {
            // Store prime numbers in the thread's local vector
            localPrimes.push_back(i);
        }
    }
}

int main(int argc, char* argv[]) {
    // Ensure correct command-line usage
    if (argc != 2) {
        std::cerr << "Incorrect Usage" << std::endl;
        return 1;
    }
    
    // Convert command-line argument to an integer
    int limit = std::stoi(argv[1]);
    
    // Determine the number of threads to use
    unsigned int numThreads = std::thread::hardware_concurrency();
    // Default the number of threads to 2 if the system does not provide a value
    if (numThreads == 0) {
        numThreads = 2;
    }
    
    // Vector to store thread objects
    std::vector<std::thread> threads;
    // Each thread gets its own vector for primes
    std::vector< std::vector<int> > threadPrimes(numThreads);
    // The range of numbers each thread will process
    int rangeSize = limit / numThreads;
    int start = 1;

    // Create and start threads
    for (unsigned int i = 0; i < numThreads; ++i) {
        int end;
        // Set the thread's range (end)
        if (i == numThreads - 1) {
            end = limit;
        } else {
            end = start + rangeSize - 1;
        }
        // Run the threads
        threads.emplace_back(findPrimes, start, end, std::ref(threadPrimes[i]));
        // Adjust the starting point of the next thread
        start = end + 1;
    }
    
    // Wait for all threads to finish
    for (auto& thread : threads) {
        thread.join();
    }
    
    // Merge the results from all threads into a single vector
    std::vector<int> primes;
    for (const auto& localPrimes : threadPrimes) {
        primes.insert(primes.end(), localPrimes.begin(), localPrimes.end());
    }
    
    // Sort the final list of primes
    std::sort(primes.begin(), primes.end());
    
    // Output the primes
    std::cout << "Prime numbers less than or equal to " << limit << ":\n";
    for (int prime : primes) {
        std::cout << prime << " ";
    }
    std::cout << std::endl;
    
    return 0;
}
