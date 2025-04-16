#include <iostream>
#include <vector>
#include <unordered_map>
#include <limits>
#include <algorithm>
using namespace std;

// Utility function to get validated integer input
int getValidatedInt(string prompt, int minVal, int maxVal) {
    int val;
    while (true) {
        cout << prompt;
        cin >> val;

        if (cin.fail() || val < minVal || val > maxVal) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between "
                 << minVal << " and " << maxVal << "." << endl;
        } else {
            break;
        }
    }
    return val;
}

// Function to get the page reference string
vector<int> getPageReferenceString() {
    int n = getValidatedInt("Enter the number of pages in the reference string: ", 1, 100);
    vector<int> pages(n);

    cout << "Enter the page reference string (space-separated non-negative integers):" << endl;
    for (int i = 0; i < n; ++i) {
        while (true) {
            cout << "\t\t Enter Reference " << i << ": ";
            cin >> pages[i];
            if (cin.fail() || pages[i] < 0) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Invalid input. Please enter a non-negative integer: ";
            } else {
                break;
            }
        }
    }
    return pages;
}

// Function to print current memory state
void printMemory(const vector<int>& memory) {
    cout << "Current Memory: [ ";
    for (int page : memory) {
        cout << page << " ";
    }
    cout << "]" << endl;
}

// LRU page replacement simulation
int simulateLRU(const vector<int>& pages, int frames) {
    vector<int> memory;
    unordered_map<int, int> lastUsed;
    int pageFaults = 0;

    cout << "\n--- LRU Simulation ---\n";
    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        cout << "Accessing page: " << page << endl;

        auto it = find(memory.begin(), memory.end(), page);
        if (it == memory.end()) {
            pageFaults++;
            cout << "Page Fault: ";
            if (memory.size() < frames) {
                memory.push_back(page);
                cout << "Inserted " << page << endl;
            } else {
                // Find least recently used
                int lruPage = memory[0], lruTime = lastUsed[lruPage];
                for (int m : memory) {
                    if (lastUsed[m] < lruTime) {
                        lruPage = m;
                        lruTime = lastUsed[m];
                    }
                }
                cout << "Replaced " << lruPage << " with " << page << endl;
                replace(memory.begin(), memory.end(), lruPage, page);
            }
        } else {
            cout << "Page Hit" << endl;
        }
        lastUsed[page] = i;
        printMemory(memory);
        cout << "----------------------\n";
    }
    cout << "Total Page Faults (LRU): " << pageFaults << endl;
    return pageFaults;
}

// Optimal page replacement simulation
int simulateOptimal(const vector<int>& pages, int frames) {
    vector<int> memory;
    int pageFaults = 0;

    cout << "\n--- Optimal Simulation ---\n";
    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        cout << "Accessing page: " << page << endl;

        auto it = find(memory.begin(), memory.end(), page);
        if (it == memory.end()) {
            pageFaults++;
            cout << "Page Fault: ";
            if (memory.size() < frames) {
                memory.push_back(page);
                cout << "Inserted " << page << endl;
            } else {
                // Find the page with the farthest future use
                int replaceIndex = -1, farthest = -1;
                for (int j = 0; j < memory.size(); ++j) {
                    int nextUse = -1;
                    for (int k = i + 1; k < pages.size(); ++k) {
                        if (pages[k] == memory[j]) {
                            nextUse = k;
                            break;
                        }
                    }
                    if (nextUse == -1) {
                        replaceIndex = j;
                        break;
                    } else if (nextUse > farthest) {
                        farthest = nextUse;
                        replaceIndex = j;
                    }
                }
                int evicted = memory[replaceIndex];
                memory[replaceIndex] = page;
                cout << "Replaced " << evicted << " with " << page << endl;
            }
        } else {
            cout << "Page Hit" << endl;
        }
        printMemory(memory);
        cout << "----------------------\n";
    }
    cout << "Total Page Faults (Optimal): " << pageFaults << endl;
    return pageFaults;
}

int simulateFifo(const vector<int>& pages, int frames) {
    vector<int> memory;
    unordered_map<int, int> lastUsed;
    int pageFaults = 0;

    cout << "\n--- LRU Simulation ---\n";
    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];
        cout << "Accessing page: " << page << endl;

        auto it = find(memory.begin(), memory.end(), page);
        if (it == memory.end()) {
            pageFaults++;
            cout << "Page Fault: ";
            if (memory.size() < frames) {
                memory.push_back(page);
                cout << "Inserted " << page << endl;
                lastUsed[page] = i;
            } else {
                // Find least recently used
                int lruPage = memory[0], lruTime = lastUsed[lruPage];
                for (int m : memory) {
                    if (lastUsed[m] < lruTime) {
                        lruPage = m;
                        lruTime = lastUsed[m];
                    }
                }
                cout << "Replaced " << lruPage << " with " << page << endl;
                replace(memory.begin(), memory.end(), lruPage, page);
            }
        } else {
            cout << "Page Hit" << endl;
        }
        printMemory(memory);
        cout << "----------------------\n";
    }
    cout << "Total Page Faults (FIFO): " << pageFaults << endl;
    return pageFaults;
}


int main() {
    cout << "====== Page Replacement Simulator ======\n\n";

    vector<int> pages = getPageReferenceString();
    int frames = getValidatedInt("Enter number of frames: ", 1, 20);

    int choice;
    do {
        cout << "\nChoose Algorithm:\n";
        cout << "1. Least Recently Used (LRU)\n";
        cout << "2. Optimal\n";
        cout << "3. Fifo\n";
        cout << "4. Exit\n";
        choice = getValidatedInt("Enter your choice (1–4): ", 1, 4);

        if (choice == 1) {
            simulateLRU(pages, frames);
        } else if (choice == 2) {
            simulateOptimal(pages, frames);
        } else if(choice == 3){
            simulateFifo(pages, frames);
        }

    } while (choice != 4);

    cout << "\nSimulation completed. Exiting.\n";
    return 0;
}
