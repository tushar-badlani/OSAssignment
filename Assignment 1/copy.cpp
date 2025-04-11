#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <source_file> <destination_file>" << endl;
        return 1;
    }

    string in = argv[1];
    string out = argv[2];

    ifstream fin(in);
    ofstream fout(out);
    string line;

    if (!fin.is_open()) {
        cerr << "Error opening source file!" << endl;
        return 1;
    }

    if (!fout.is_open()) {
        cerr << "Error opening destination file!" << endl;
        return 1;
    }

    while (getline(fin, line)) {
        fout << line << endl;
    }

    fin.close();
    fout.close();
    cout << "File copied successfully!" << endl;
    return 0;
}

