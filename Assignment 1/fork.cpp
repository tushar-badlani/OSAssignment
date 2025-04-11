#include <iostream>
#include <unistd.h>  
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

int main() {
    string in, out, pattern;

    cout << "Enter source file: ";
    cin >> in;

    cout << "What do you want to do?" << endl;
    cout << "1. Copy the file" << endl;
    cout << "2. Search for a pattern in the file" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;

    pid_t pid = fork();

    if (pid < 0) {
        cerr << "Fork failed!" << endl;
        return 1;
    } else if (pid == 0) {
        // Child process
        cout << "Child Process ID: " << getpid() << ", Parent Process ID: " << getppid() << endl;

        if (choice == 1) {
            cout << "Enter destination file: ";
            cin >> out;

            // Use exec to run the copy program
            char* args[] = {(char*)"./copy", (char*)in.c_str(), (char*)out.c_str(), NULL};
            execvp(args[0], args);
        } else if (choice == 2) {
            cout << "Enter pattern to search: ";
            cin >> pattern;

            cout << "Do you want to search with options? (y/n): ";
            char optionChoice;
            cin >> optionChoice;

            if (optionChoice == 'y' || optionChoice == 'Y') {
                cout << "Options: (-i for case-insensitive search, -n for line numbers)" << endl;
                cout << "Enter options (separate with space): ";
                string options;
                cin.ignore();
                getline(cin, options);  // Get options entered by the user

                // Execute the grep program with options
                char* args[] = {(char*)"./grep", (char*)in.c_str(), (char*)pattern.c_str(), 
                                 (char*)options.c_str(), NULL};
                execvp(args[0], args);
            } else {
                // Default grep without options
                char* args[] = {(char*)"./grep", (char*)in.c_str(), (char*)pattern.c_str(), NULL};
                execvp(args[0], args);
            }
        } else {
            cerr << "Invalid choice!" << endl;
        }
    } else {
        // Parent process
        wait(NULL); // Wait for the child process to finish
        cout << "Parent Process ID: " << getpid() << ", Child Process ID: " << pid << endl;

        cout << "Operation completed!" << endl;
    }

    return 0;
}

