#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// Case-insensitive search
bool caseInsensitiveFind(const string& line, const string& pattern) {
    string lowerLine = line;
    string lowerPattern = pattern;

    transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
    transform(lowerPattern.begin(), lowerPattern.end(), lowerPattern.begin(), ::tolower);

    return lowerLine.find(lowerPattern) != string::npos;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <file> <pattern> [-v] [-c]" << endl;
        return 1;
    }

    string filename = argv[1];
    string pattern = argv[2];
    bool countOnly = false;
    bool invertMatch = false;

    // Parse options
    for (int i = 3; i < argc; i++) {
        string option = argv[i];
        if (option == "-c") {
            countOnly = true;
        } else if (option == "-v") {
            invertMatch = true;
        } else {
            cerr << "Unknown option: " << option << endl;
            return 1;
        }
    }

    ifstream fin(filename);
    if (!fin) {
        cerr << "Error opening file: " << filename << endl;
        return 1;
    }

    string line;
    int matchCount = 0;

    while (getline(fin, line)) {
        bool match = line.find(pattern) != string::npos;
        if (invertMatch) match = !match;

        if (match) {
            matchCount++;
            if (!countOnly) {
                cout << line << endl;
            }
        }
    }

    if (countOnly) {
        cout << "Match count: " << matchCount << endl;
    }

    if (!countOnly && matchCount == 0) {
        cout << "Pattern not found in the file." << endl;
    }

    fin.close();
    return 0;
}
