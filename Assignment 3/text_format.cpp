#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// Function to convert text to uppercase
string toUppercase(const string& text) {
    string result = "";
    for (char c : text) {
        result += toupper(c);
    }
    return result;
}

// Function to convert text to lowercase
string toLowercase(const string& text) {
    string result = "";
    for (char c : text) {
        result += tolower(c);
    }
    return result;
}

// Function to remove extra whitespace
string removeWhitespace(const string& text) {
    string result = "";
    bool spaceFound = false;
    for (char c : text) {
        if (isspace(c)) {
            if (!spaceFound) {
                result += ' ';
                spaceFound = true;
            }
        } else {
            result += c;
            spaceFound = false;
        }
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <option>" << endl;
        cerr << "Options: -u (uppercase), -l (lowercase), -w (whitespace removal)" << endl;
        return 1;
    }

    string option = argv[1];
    int pipefd[2]; // Pipe file descriptors
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close write end
        dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to read end of pipe
        close(pipefd[0]);

        string line;
        while (getline(cin, line)) {
            if (option == "-u") {
                cout << toUppercase(line) << endl;
            } else if (option == "-l") {
                cout << toLowercase(line) << endl;
            } else if (option == "-w") {
                cout << removeWhitespace(line) << endl;
            } else {
                cerr << "Invalid option: " << option << endl;
                return 1;
            }
        }
        return 0;
    } else { // Parent process
        close(pipefd[0]); // Close read end
        string inputLine;
        while (getline(cin, inputLine)) {
            write(pipefd[1], inputLine.c_str(), inputLine.length());
            write(pipefd[1], "\n", 1);
        }
        close(pipefd[1]); // Close write end
        wait(NULL); // Wait for child process to complete
    }

    return 0;
}


