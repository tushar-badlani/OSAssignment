#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed.\n";
        return 1;
    }

    if (pid == 0) {
        // Child process
        cout << "Child Process ID: " << getpid() << ", Parent Process ID: " << getppid() << '\n';

        int choice;
        cout << "1. Copy file\n2. Grep in file\nEnter your choice: ";
        cin >> choice;

        if (choice == 1) {
            execl("./copy", "copy", (char*)NULL);
        } else if (choice == 2) {
            execl("./grep", "grep", (char*)NULL);
        } else {
            cout << "Invalid choice.\n";
        }
    } else {
        // Parent process
        cout << "Parent Process ID: " << getpid() << ", Child Process ID: " << pid << '\n';
        wait(NULL); // wait for child to finish
        cout << "Child process completed.\n";
    }

    return 0;
}
