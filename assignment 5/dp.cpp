#include <iostream>
#include <vector>
#include <stdexcept>
#include <unordered_map>
#include <string>
using namespace std;

class BankersAlgorithm {
private:
    int numProcesses, numResources;
    vector<int> available;
    vector<vector<int>> maxDemand, allocation, need;
    unordered_map<string, vector<vector<int>>> memo;

    string createStateKey(const vector<int>& avl, const vector<bool>& finish) {
        string key;
        for (int w : avl) key += to_string(w) + ",";
        key += "|";
        for (bool f : finish) key += (f ? "1" : "0");
        return key;
    }

    vector<vector<int>> findSafeSequencesDP(vector<int> avl, vector<bool> finish) {
        string stateKey = createStateKey(avl, finish);
        if (memo.find(stateKey) != memo.end()) {
            return memo[stateKey];
        }
        
        bool allFinished = true;
        for (bool f : finish) {
            if (!f) {
                allFinished = false;
                break;
            }
        }
        
        if (allFinished) {
            return {{}};
        }
        
        vector<vector<int>> results;
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
                    vector<int> newavl = avl;
                    vector<bool> newFinish = finish;
                    for (int j = 0; j < numResources; j++) {
                        newavl[j] += allocation[p][j];
                    }
                    newFinish[p] = true;
                    vector<vector<int>> subResults = findSafeSequencesDP(newavl, newFinish);
                    for (auto& seq : subResults) {
                        vector<int> newSeq = {p};
                        newSeq.insert(newSeq.end(), seq.begin(), seq.end());
                        results.push_back(newSeq);
                    }
                }
            }
        }
        memo[stateKey] = results;
        return results;
    }

public:
    BankersAlgorithm(int processes, int resources)
        : numProcesses(processes), numResources(resources), available(resources) {
        maxDemand.assign(numProcesses, vector<int>(numResources));
        allocation.assign(numProcesses, vector<int>(numResources, 0));
        need.assign(numProcesses, vector<int>(numResources));
    }

    void initialize() {
        cout << "Enter available resources (space-separated integers): ";
        for (int &i : available) {
            while (!(cin >> i) || i < 0) {
                cout << "Invalid input. Enter a non-negative integer: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        
        cout << "Enter maximum demand matrix (each row separated by new line):\n";
        for (auto &row : maxDemand) {
            for (int &val : row) {
                while (!(cin >> val) || val < 0) {
                    cout << "Invalid input. Enter a non-negative integer: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }
        
        cout << "Enter allocation matrix (each row separated by new line):\n";
        for (auto &row : allocation) {
            for (int &val : row) {
                while (!(cin >> val) || val < 0) {
                    cout << "Invalid input. Enter a non-negative integer: ";
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                }
            }
        }
        
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                need[i][j] = maxDemand[i][j] - allocation[i][j];
                if (need[i][j] < 0) {
                    cout << "Error: Need cannot be negative. Please re-enter allocation values.\n";
                    initialize();
                    return;
                }
            }
        }
    }

    void printState() {
        cout << "\nAvailable Resources: ";
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
        memo.clear();
        vector<int> avl = available;
        vector<bool> finish(numProcesses, false);
        vector<vector<int>> allSeqs = findSafeSequencesDP(avl, finish);
        
        if (allSeqs.empty()) {
            cout << "No safe sequence found. The system is in an unsafe state.\n";
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
    cout << "Enter number of processes and resources: ";
    while (!(cin >> processes >> resources) || processes <= 0 || resources <= 0) {
        cout << "Invalid input. Enter two positive integers for processes and resources: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    BankersAlgorithm banker(processes, resources);
    banker.initialize();
    
    cout << "\nInitial State:\n";
    banker.printState();
    banker.findAllSafeSequences();
    
    return 0;
}
