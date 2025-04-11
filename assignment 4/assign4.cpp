#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

using namespace std;

int balance = 1000;
int readcount = 0;

// Synchronization primitives
mutex mtx;
mutex write_mtx;
condition_variable cv;

// ---------------------------- Incorrect ----------------------------
void incorrect_check_balance(int id) {
    cout << "[Incorrect] Customer " << id << " is checking account balance" << endl;
    readcount++;
    if (readcount == 1) {
        cout << "[Incorrect] Customer " << id << " assumes it's the first reader, but no lock" << endl;
    }

    int temp = balance;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "[Incorrect] Customer " << id << " sees balance = Rs." << temp << endl;
    this_thread::sleep_for(chrono::seconds(1));

    readcount--;
    if (readcount == 0) {
        cout << "[Incorrect] Customer " << id << " done checking, but still no lock" << endl;
    }
}

void incorrect_transaction(int id) {
    cout << "[Incorrect] Customer " << id << " is depositing Rs.500" << endl;
    int temp = balance;
    temp += 500;
    this_thread::sleep_for(chrono::milliseconds(500));
    balance = temp;
    cout << "[Incorrect] Customer " << id << " new balance after deposit = Rs." << balance << endl;
    this_thread::sleep_for(chrono::seconds(1));
}

// ---------------------------- Correct ----------------------------
void correct_check_balance(int id) {
    cout << "[Correct] Customer " << id << " is checking account balance" << endl;

    {
        unique_lock<mutex> lock(mtx);
        readcount++;
        if (readcount == 1) {
            cout << "[Correct] Customer " << id << " is the first reader, blocking transactions" << endl;
            write_mtx.lock();
        }
    }

    int temp = balance;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "[Correct] Customer " << id << " sees balance = Rs." << temp << endl;
    this_thread::sleep_for(chrono::seconds(1));

    {
        unique_lock<mutex> lock(mtx);
        readcount--;
        if (readcount == 0) {
            cout << "[Correct] Customer " << id << " is the last reader, allowing transactions" << endl;
            write_mtx.unlock();
        }
    }
}

void correct_transaction(int id) {
    cout << "[Correct] Customer " << id << " is depositing Rs.500" << endl;
    write_mtx.lock();
    int temp = balance;
    temp += 500;
    this_thread::sleep_for(chrono::milliseconds(500));
    balance = temp;
    cout << "[Correct] Customer " << id << " new balance after deposit = Rs." << balance << endl;
    this_thread::sleep_for(chrono::seconds(1));
    write_mtx.unlock();
}

// ---------------------------- Application: Banking - Deposit and Check Balance ----------------------------
int main() {
    thread r[2], w[2];
    int ids[] = {1, 2};

    cout << "\n-- Correct Synchronization: Banking System with Proper Locks --\n";

    cout << "\nCase 1: One Customer Checking, One Depositing (simultaneously)\n";
    r[0] = thread(correct_check_balance, ids[0]);
    w[0] = thread(correct_transaction, ids[0]);
    r[0].join();
    w[0].join();

    cout << "\nCase 2: Two Depositors (simultaneously)\n";
    w[0] = thread(correct_transaction, ids[0]);
    w[1] = thread(correct_transaction, ids[1]);
    w[0].join();
    w[1].join();

    cout << "\nCase 3: Two Balance Checks (simultaneously)\n";
    r[0] = thread(correct_check_balance, ids[0]);
    r[1] = thread(correct_check_balance, ids[1]);
    r[0].join();
    r[1].join();

    cout << "\nCase 4: One Depositor, One Balance Check (simultaneously)\n";
    w[0] = thread(correct_transaction, ids[0]);
    r[0] = thread(correct_check_balance, ids[0]);
    w[0].join();
    r[0].join();

    cout << "\n-- Incorrect Synchronization: Banking System without Proper Locks --\n";

    cout << "\nCase 1: One Customer Checking, One Depositing (simultaneously)\n";
    r[0] = thread(incorrect_check_balance, ids[0]);
    w[0] = thread(incorrect_transaction, ids[0]);
    r[0].join();
    w[0].join();

    cout << "\nCase 2: Two Depositors (simultaneously)\n";
    w[0] = thread(incorrect_transaction, ids[0]);
    w[1] = thread(incorrect_transaction, ids[1]);
    w[0].join();
    w[1].join();

    cout << "\nCase 3: Two Balance Checks (simultaneously)\n";
    r[0] = thread(incorrect_check_balance, ids[0]);
    r[1] = thread(incorrect_check_balance, ids[1]);
    r[0].join();
    r[1].join();

    cout << "\nCase 4: One Depositor, One Balance Check (simultaneously)\n";
    w[0] = thread(incorrect_transaction, ids[0]);
    r[0] = thread(incorrect_check_balance, ids[0]);
    w[0].join();
    r[0].join();

    return 0;
}
