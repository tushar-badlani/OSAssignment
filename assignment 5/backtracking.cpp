#include <iostream>
#include <vector>
#include <limits>
using namespace std;

class BankersAlgorithm {
private:
    int numProcesses, numResources;
    vector<int> available;
    vector<vector<int>> maxDemand, allocation, need;

    void findSafeSequences(vector<int>& avl, vector<bool>& finish, vector<int>& safeSeq, vector<vector<int>>& allSeqs) {
        bool found = false;
        for (int p = 0; p < numProcesses; p++) {
            if (!finish[p]) {
                bool canProceed = true;
                for (int j = 0; j < numResources; j++) {
                    if (need[p][j] > avl[j]) {
                        canProceed = false;
                        break;
                    }
                }
                if (canProceed) {
                    for (int j = 0; j < numResources; j++) {
                        avl[j] += allocation[p][j];
                    }
                    finish[p] = true;
                    safeSeq.push_back(p);
                    findSafeSequences(avl, finish, safeSeq, allSeqs);
                    safeSeq.pop_back();
                    finish[p] = false;
                    for (int j = 0; j < numResources; j++) {
                        avl[j] -= allocation[p][j];
                    }
                    found = true;
                }
            }
        }
        if (!found) {
            allSeqs.push_back(safeSeq);
        }
    }

public:
    BankersAlgorithm(int processes, int resources)
        : numProcesses(processes), numResources(resources), available(resources) {
        maxDemand.assign(numProcesses, vector<int>(numResources));
        allocation.assign(numProcesses, vector<int>(numResources, 0));
        need.assign(numProcesses, vector<int>(numResources));
    }

    void getValidInput(int& value, const string& message) {
        while (true) {
            cout << message;
            if (cin >> value && value >= 0) break;
            cout << "Invalid input. Please enter a non-negative integer.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void initialize() {
        cout << "\nEnter available resources for each type (should be non-negative integers):\n";
        for (int &i : available) {
            getValidInput(i, "Enter available resource: ");
        }

        cout << "\nEnter the Maximum Demand Matrix (each cell represents maximum demand of a process for a resource):\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "For Process P" << i << ":\n";
            for (int &val : maxDemand[i]) {
                getValidInput(val, "Max Demand: ");
            }
        }

        cout << "\nEnter the Allocation Matrix (each cell represents currently allocated resources):\n";
        for (int i = 0; i < numProcesses; i++) {
            cout << "For Process P" << i << ":\n";
            for (int &val : allocation[i]) {
                getValidInput(val, "Allocated: ");
            }
        }

        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                need[i][j] = maxDemand[i][j] - allocation[i][j];
                if (need[i][j] < 0) {
                    cout << "Error: Need cannot be negative. Check allocation and max demand again.\n";
                    exit(EXIT_FAILURE);
                }
            }
        }
    }

    void printState() {
        cout << "\nCurrent System State:\n";
        cout << "Available Resources:\n";
        for (int i : available) cout << i << " ";
        cout << "\n\nAllocation Matrix:\n";
        for (auto& row : allocation) {
            for (int val : row) cout << val << " ";
            cout << "\n";
        }
        cout << "\nNeed Matrix:\n";
        for (auto& row : need) {
            for (int val : row) cout << val << " ";
            cout << "\n";
        }
    }

    void findAllSafeSequences() {
        vector<int> avl = available;
        vector<bool> finish(numProcesses, false);
        vector<int> safeSeq;
        vector<vector<int>> allSeqs;
        findSafeSequences(avl, finish, safeSeq, allSeqs);

        if (allSeqs.empty()) {
            cout << "\nNo safe sequence found. The system is in an unsafe state.\n";
        } else {
            cout << "\nAll possible safe sequences:\n";
            for (const auto& seq : allSeqs) {
                for (int p : seq) {
                    cout << "P" << p << " ";
                }
                cout << "\n";
            }
        }
    }
};

int main() {
    int processes, resources;
    while (true) {
        cout << "\nEnter the number of processes (must be positive): ";
        if (cin >> processes && processes > 0) break;
        cout << "Invalid input. Please enter a positive integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    while (true) {
        cout << "Enter the number of resource types (must be positive): ";
        if (cin >> resources && resources > 0) break;
        cout << "Invalid input. Please enter a positive integer.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    BankersAlgorithm banker(processes, resources);
    banker.initialize();
    banker.printState();
    banker.findAllSafeSequences();
    return 0;
}
