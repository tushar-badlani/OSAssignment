#include <bits/stdc++.h>
#include <unistd.h>
using namespace std;

int main() {
    string src, dest;
    cout << "Enter source file name: ";
    cin >> src;
    cout << "Enter destination file name: ";
    cin >> dest;

    ifstream inFile(src);
    ofstream outFile(dest);

    if (!inFile) {
        cerr << "Cannot open source file.\n";
        return 1;
    }
    if (!outFile) {
        cerr << "Cannot create destination file.\n";
        return 1;
    }

    string line;
    while (getline(inFile, line)) {
        outFile << line << '\n';
    }

    inFile.close();
    outFile.close();

    cout << "File copied successfully.\n";
    return 0;
}
