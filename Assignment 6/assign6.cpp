#include <iostream>
#include <vector>
#include <iomanip>
#include <limits>
#include <string>
#include <algorithm>
using namespace std;

struct AllocationResult {
    string name;
    int allocated;
    int total_fragmentation;
    double efficiency;
};

void displayTable(const vector<int> &processes, const vector<int> &allocation, const vector<int> &fragments) {
    cout << "\nProcess Allocation Table:\n";
    cout << "------------------------------------------\n";
    cout << "Process | Process Size | Block No | Fragment\n";
    cout << "------------------------------------------\n";
    for (size_t i = 0; i < processes.size(); i++) {
        cout << setw(7) << i + 1 << " | "
             << setw(12) << processes[i] << " | "
             << setw(8) << (allocation[i] != -1 ? to_string(allocation[i] + 1) : "N/A") << " | "
             << (allocation[i] != -1 ? to_string(fragments[i]) : "-") << "\n";
    }
}

AllocationResult firstFit(vector<int> blocks, vector<int> processes, bool showTable = true) {
    vector<int> allocation(processes.size(), -1), fragments(processes.size(), 0);
    int allocated = 0;
    int total_fragmentation = 0;
    
    for (size_t i = 0; i < processes.size(); i++) {
        for (size_t j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i]) {
                allocation[i] = j;
                fragments[i] = blocks[j] - processes[i];
                total_fragmentation += fragments[i];
                blocks[j] -= processes[i];
                allocated++;
                break;
            }
        }
    }
    
    if (showTable) displayTable(processes, allocation, fragments);
    double efficiency = (allocated * 100.0) / processes.size();
    return {"First Fit", allocated, total_fragmentation, efficiency};
}

AllocationResult bestFit(vector<int> blocks, vector<int> processes, bool showTable = true) {
    vector<int> allocation(processes.size(), -1), fragments(processes.size(), 0);
    int allocated = 0;
    int total_fragmentation = 0;
    
    for (size_t i = 0; i < processes.size(); i++) {
        int bestIdx = -1;
        for (size_t j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i] && (bestIdx == -1 || blocks[j] < blocks[bestIdx])) {
                bestIdx = j;
            }
        }
        if (bestIdx != -1) {
            allocation[i] = bestIdx;
            fragments[i] = blocks[bestIdx] - processes[i];
            total_fragmentation += fragments[i];
            blocks[bestIdx] -= processes[i];
            allocated++;
        }
    }
    
    if (showTable) displayTable(processes, allocation, fragments);
    double efficiency = (allocated * 100.0) / processes.size();
    return {"Best Fit", allocated, total_fragmentation, efficiency};
}

AllocationResult worstFit(vector<int> blocks, vector<int> processes, bool showTable = true) {
    vector<int> allocation(processes.size(), -1), fragments(processes.size(), 0);
    int allocated = 0;
    int total_fragmentation = 0;
    
    for (size_t i = 0; i < processes.size(); i++) {
        int worstIdx = -1;
        for (size_t j = 0; j < blocks.size(); j++) {
            if (blocks[j] >= processes[i] && (worstIdx == -1 || blocks[j] > blocks[worstIdx])) {
                worstIdx = j;
            }
        }
        if (worstIdx != -1) {
            allocation[i] = worstIdx;
            fragments[i] = blocks[worstIdx] - processes[i];
            total_fragmentation += fragments[i];
            blocks[worstIdx] -= processes[i];
            allocated++;
        }
    }
    
    if (showTable) displayTable(processes, allocation, fragments);
    double efficiency = (allocated * 100.0) / processes.size();
    return {"Worst Fit", allocated, total_fragmentation, efficiency};
}

AllocationResult nextFit(vector<int> blocks, vector<int> processes, bool showTable = true) {
    vector<int> allocation(processes.size(), -1), fragments(processes.size(), 0);
    int lastAllocated = 0;
    int allocated = 0;
    int total_fragmentation = 0;
    
    for (size_t i = 0; i < processes.size(); i++) {
        size_t j = lastAllocated;
        do {
            if (blocks[j] >= processes[i]) {
                allocation[i] = j;
                fragments[i] = blocks[j] - processes[i];
                total_fragmentation += fragments[i];
                blocks[j] -= processes[i];
                lastAllocated = j;
                allocated++;
                break;
            }
            j = (j + 1) % blocks.size();
        } while (j != lastAllocated);
    }
    
    if (showTable) displayTable(processes, allocation, fragments);
    double efficiency = (allocated * 100.0) / processes.size();
    return {"Next Fit", allocated, total_fragmentation, efficiency};
}

void analyzePerformance(const vector<AllocationResult>& results) {
    cout << "\n--- Performance Analysis ---\n";
    cout << "-------------------------------------------------\n";
    cout << "Algorithm    | Allocated | Total Fragmentation | Efficiency\n";
    cout << "-------------------------------------------------\n";
    
    for (const auto& result : results) {
        cout << left << setw(12) << result.name << " | "
             << setw(9) << result.allocated << " | "
             << setw(19) << result.total_fragmentation << " | "
             << fixed << setprecision(2) << result.efficiency << "%\n";
    }
    
    // Find best algorithm (prioritize allocation, then efficiency, then fragmentation)
    auto best = max_element(results.begin(), results.end(), 
        [](const AllocationResult& a, const AllocationResult& b) {
            if (a.allocated != b.allocated) return a.allocated < b.allocated;
            if (a.efficiency != b.efficiency) return a.efficiency < b.efficiency;
            return a.total_fragmentation > b.total_fragmentation;
        });
    
    cout << "\nBest algorithm for this scenario: " << best->name 
         << " (Allocated " << best->allocated << " processes, " 
         << fixed << setprecision(2) << best->efficiency << "% efficiency)\n";
}

int main() {
    int numBlocks, numProcesses;
    
    do {
        cout << "Enter number of memory blocks: ";
        cin >> numBlocks;
        if (cin.fail() || numBlocks <= 0) {
            cout << "Invalid input! Please enter a positive integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (numBlocks <= 0);
    
    vector<int> blocks(numBlocks);
    for (int i = 0; i < numBlocks; ++i) {
        do {
            cout << "Enter block size for block " << i + 1 << ": ";
            cin >> blocks[i];
            if (cin.fail() || blocks[i] <= 0) {
                cout << "Invalid input! Please enter a positive integer.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (blocks[i] <= 0);
    }

    do {
        cout << "Enter number of processes: ";
        cin >> numProcesses;
        if (cin.fail() || numProcesses <= 0) {
            cout << "Invalid input! Please enter a positive integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } while (numProcesses <= 0);
    
    vector<int> processes(numProcesses);
    for (int i = 0; i < numProcesses; ++i) {
        do {
            cout << "Enter process size for process " << i + 1 << ": ";
            cin >> processes[i];
            if (cin.fail() || processes[i] <= 0) {
                cout << "Invalid input! Please enter a positive integer.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (processes[i] <= 0);
    }

    // Run all algorithms silently first to analyze performance
    vector<AllocationResult> results;
    results.push_back(firstFit(blocks, processes, false));
    results.push_back(bestFit(blocks, processes, false));
    results.push_back(worstFit(blocks, processes, false));
    results.push_back(nextFit(blocks, processes, false));
    
    // Display performance analysis
    analyzePerformance(results);

    // Menu for detailed view
    int choice;
    do {
        cout << "\n--- Memory Allocation Techniques ---\n";
        cout << "1. First Fit Detailed View\n";
        cout << "2. Best Fit Detailed View\n";
        cout << "3. Worst Fit Detailed View\n";
        cout << "4. Next Fit Detailed View\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || choice < 1 || choice > 5) {
            cout << "Invalid choice! Please enter a number between 1 and 5.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                firstFit(blocks, processes, true);
                break;
            case 2:
                bestFit(blocks, processes, true);
                break;
            case 3:
                worstFit(blocks, processes, true);
                break;
            case 4:
                nextFit(blocks, processes, true);
                break;
            case 5:
                cout << "Exiting...\n";
                break;
        }
    } while (choice != 5);

    return 0;
}