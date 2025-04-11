#include <iostream>
#include <pthread.h>
#include <unistd.h>    // for usleep()
#include <stdexcept>   // for std::runtime_error
#include <cstdlib>
#include <mutex>       // for std::mutex (to help control output order)

// Shared bank balance variable
int bankBalance = 1000; // starting balance

// Number of iterations for each thread operation
const int NUM_ITERATIONS = 5;

// Mutex to help control output order (for demonstration purposes)
std::mutex consoleMutex;

// ***********************
// *** UNSYNCHRONIZED  ***
// ***********************

// Writer function (deposit and withdrawal) without synchronization
void* unsync_transaction(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        // Simulate random operation: deposit if even iteration, withdrawal if odd
        if (i % 2 == 0) {
            // Deposit operation: add 100
            usleep(120000);
            bankBalance += 100;
           
            // Use mutex to ensure clean console output
            consoleMutex.lock();
            std::cout << "[Unsync Transaction " << id << "] Deposited 100. New Balance: " << bankBalance << std::endl;
            consoleMutex.unlock();
        } else {
            // Withdrawal operation: subtract 150
            usleep(150000);
            bankBalance -= 150;
           
            // Use mutex to ensure clean console output
            consoleMutex.lock();
            std::cout << "[Unsync Transaction " << id << "] Withdrew 150. New Balance: " << bankBalance << std::endl;
            consoleMutex.unlock();
        }
    }
    return nullptr;
}

// Reader function (balance check) without synchronization
void* unsync_balance_check(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < NUM_ITERATIONS * 2; ++i) { // more frequent balance checks
        usleep(100000);
       
        // Use mutex to ensure clean console output
        consoleMutex.lock();
        std::cout << "[Unsync Balance Reader " << id << "] Checked Balance: " << bankBalance << std::endl;
        consoleMutex.unlock();
    }
    return nullptr;
}

// ***********************
// *** SYNCHRONIZED    ***
// ***********************

// Declare a read-write lock for protecting bankBalance
pthread_rwlock_t rwlock;

// Writer function (deposit and withdrawal) with synchronization
void* sync_transaction(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        // For each iteration, decide operation type: deposit if even, withdrawal if odd.
        if (i % 2 == 0) {
            // Acquire write lock for deposit operation
            if (pthread_rwlock_wrlock(&rwlock) != 0) {
                std::cerr << "[Sync Transaction " << id << "] Failed to acquire write lock for deposit." << std::endl;
                pthread_exit(nullptr);
            }
           
            // Sleep for the same duration as unsynchronized version
            usleep(120000);
           
            // Deposit operation: add 100
            bankBalance += 100;
           
            // Use mutex to ensure clean console output
            consoleMutex.lock();
            std::cout << "[Sync Transaction " << id << "] Deposited 100. New Balance: " << bankBalance << std::endl;
            consoleMutex.unlock();
           
            if (pthread_rwlock_unlock(&rwlock) != 0) {
                std::cerr << "[Sync Transaction " << id << "] Failed to release write lock after deposit." << std::endl;
                pthread_exit(nullptr);
            }
        } else {
            // Acquire write lock for withdrawal operation
            if (pthread_rwlock_wrlock(&rwlock) != 0) {
                std::cerr << "[Sync Transaction " << id << "] Failed to acquire write lock for withdrawal." << std::endl;
                pthread_exit(nullptr);
            }
           
            // Sleep for the same duration as unsynchronized version
            usleep(150000);
           
            // Withdrawal operation: subtract 150
            bankBalance -= 150;
           
            // Use mutex to ensure clean console output
            consoleMutex.lock();
            std::cout << "[Sync Transaction " << id << "] Withdrew 150. New Balance: " << bankBalance << std::endl;
            consoleMutex.unlock();
           
            if (pthread_rwlock_unlock(&rwlock) != 0) {
                std::cerr << "[Sync Transaction " << id << "] Failed to release write lock after withdrawal." << std::endl;
                pthread_exit(nullptr);
            }
        }
    }
    return nullptr;
}

// Reader function (balance check) with synchronization
void* sync_balance_check(void* arg) {
    int id = *((int*)arg);
    for (int i = 0; i < NUM_ITERATIONS * 2; ++i) {
        // Sleep for the same duration as unsynchronized version
        usleep(100000);
       
        // Acquire read lock to safely check balance
        if (pthread_rwlock_rdlock(&rwlock) != 0) {
            std::cerr << "[Sync Balance Reader " << id << "] Failed to acquire read lock." << std::endl;
            pthread_exit(nullptr);
        }
       
        // Use mutex to ensure clean console output
        consoleMutex.lock();
        std::cout << "[Sync Balance Reader " << id << "] Checked Balance: " << bankBalance << std::endl;
        consoleMutex.unlock();
       
        if (pthread_rwlock_unlock(&rwlock) != 0) {
            std::cerr << "[Sync Balance Reader " << id << "] Failed to release read lock." << std::endl;
            pthread_exit(nullptr);
        }
    }
    return nullptr;
}

int main() {
    const int NUM_TRANSACTIONS = 3;  // number of transaction (writer) threads
    const int NUM_READERS = 2;       // number of balance checking (reader) threads
    pthread_t transThreads[NUM_TRANSACTIONS];
    pthread_t readerThreads[NUM_READERS];
    int transIDs[NUM_TRANSACTIONS];
    int readerIDs[NUM_READERS];

    // -----------------------------
    // Part 1: Without Synchronization
    // -----------------------------
    std::cout << "=== Part 1: Running Without Synchronization ===" << std::endl;
    bankBalance = 1000; // reset starting balance

    // Create transaction threads (deposit/withdraw) without synchronization
    for (int i = 0; i < NUM_TRANSACTIONS; ++i) {
        transIDs[i] = i + 1;
        try {
            if (pthread_create(&transThreads[i], nullptr, unsync_transaction, &transIDs[i]) != 0) {
                throw std::runtime_error("Failed to create unsynchronized transaction thread");
            }
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Create balance checking threads without synchronization
    for (int i = 0; i < NUM_READERS; ++i) {
        readerIDs[i] = i + 1;
        try {
            if (pthread_create(&readerThreads[i], nullptr, unsync_balance_check, &readerIDs[i]) != 0) {
                throw std::runtime_error("Failed to create unsynchronized balance reader thread");
            }
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Wait for unsynchronized transaction threads to finish
    for (int i = 0; i < NUM_TRANSACTIONS; ++i) {
        pthread_join(transThreads[i], nullptr);
    }
    // Wait for unsynchronized reader threads to finish
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_join(readerThreads[i], nullptr);
    }

    std::cout << "\nFinal Bank Balance without Sync: " << bankBalance << std::endl;
    std::cout << "Notice that due to race conditions, the final balance may be inconsistent.\n" << std::endl;

    // -----------------------------
    // Part 2: With Synchronization
    // -----------------------------
    std::cout << "=== Part 2: Running With Synchronization ===" << std::endl;
    bankBalance = 1000; // reset starting balance

    // Initialize the read-write lock
    if (pthread_rwlock_init(&rwlock, nullptr) != 0) {
        std::cerr << "Failed to initialize read-write lock." << std::endl;
        exit(EXIT_FAILURE);
    }

    // Create synchronized transaction threads (deposit/withdraw)
    for (int i = 0; i < NUM_TRANSACTIONS; ++i) {
        transIDs[i] = i + 1;
        try {
            if (pthread_create(&transThreads[i], nullptr, sync_transaction, &transIDs[i]) != 0) {
                throw std::runtime_error("Failed to create synchronized transaction thread");
            }
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Create synchronized balance checking threads
    for (int i = 0; i < NUM_READERS; ++i) {
        readerIDs[i] = i + 1;
        try {
            if (pthread_create(&readerThreads[i], nullptr, sync_balance_check, &readerIDs[i]) != 0) {
                throw std::runtime_error("Failed to create synchronized balance reader thread");
            }
        } catch (const std::exception &ex) {
            std::cerr << ex.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Wait for synchronized transaction threads to finish
    for (int i = 0; i < NUM_TRANSACTIONS; ++i) {
        pthread_join(transThreads[i], nullptr);
    }
    // Wait for synchronized reader threads to finish
    for (int i = 0; i < NUM_READERS; ++i) {
        pthread_join(readerThreads[i], nullptr);
    }

    // Destroy the read-write lock
    if (pthread_rwlock_destroy(&rwlock) != 0) {
        std::cerr << "Failed to destroy read-write lock." << std::endl;
        exit(EXIT_FAILURE);
    }

    std::cout << "\nFinal Bank Balance with Sync: " << bankBalance << std::endl;
    std::cout << "With synchronization, the operations are consistent and the final balance is reliable." << std::endl;

    return 0;
}