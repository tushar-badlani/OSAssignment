#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

int main() {
    string filename, keyword;
    cout << "Enter file name: ";
    cin >> filename;
    cout << "Enter keyword to search: ";
    cin >> keyword;

    ifstream file(filename);
    if (!file) {
        cerr << "Cannot open file.\n";
        return 1;
    }

    string line;
    int lineNum = 0;
    int count = 0;
    cout << "Searching for keyword: " << keyword << '\n';
    while (getline(file, line)) {
        lineNum++;

        if (line.find(keyword) != string::npos) {
            cout << "Line " << lineNum << ": " << line << '\n';
            count++;
        }
    }

    if (count == 0) {
        cout << "Keyword not found in the file.\n";
    } else {
        cout << "Total occurrences of keyword: " << count << '\n';
    }
    cout << "Search completed.\n";
    // Close the file

    file.close();
    return 0;
}
