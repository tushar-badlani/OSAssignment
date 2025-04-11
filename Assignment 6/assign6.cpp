#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct MemoryBlock {
    int blockId;
    int size;
    bool allocated;
    string process;
};


void printMemory(const vector<MemoryBlock>& memory) {
    cout << "\nMemory Status:\n";
    cout << "Block ID\tSize\t\tStatus\t\tProcess\n";
    for (const auto& block : memory) {
        cout << block.blockId << "\t\t" << block.size << "\t\t"
             << (block.allocated ? "Allocated" : "Free") << "\t\t"
             << (block.allocated ? block.process : "-") << endl;
    }
}

void resetMemory(vector<MemoryBlock>& memory, const vector<MemoryBlock>& original) {
    memory = original;
}

bool firstFit(vector<MemoryBlock>& memory, int processSize, const string& processName) {
    for (int i = 0; i < memory.size(); i++) {
        if (!memory[i].allocated && memory[i].size >= processSize) {
            memory[i].allocated = true;
            memory[i].process = processName;
            if (memory[i].size > processSize) {
                MemoryBlock newBlock = {memory[i].blockId, memory[i].size - processSize, false, ""};
                memory[i].size = processSize;
                memory.insert(memory.begin() + i + 1, newBlock);
            }
            return true;
        }
    }
    return false;
}

bool bestFit(vector<MemoryBlock>& memory, int processSize, const string& processName) {
    int bestIdx = -1;
    for (int i = 0; i < memory.size(); i++) {
        if (!memory[i].allocated && memory[i].size >= processSize) {
            if (bestIdx == -1 || memory[i].size < memory[bestIdx].size) {
                bestIdx = i;
            }
        }
    }

    if (bestIdx != -1) {
        memory[bestIdx].allocated = true;
        memory[bestIdx].process = processName;
        if (memory[bestIdx].size > processSize) {
            MemoryBlock newBlock = {
                memory[bestIdx].blockId,                // same blockId
                memory[bestIdx].size - processSize,
                false,
                ""
            };
            memory[bestIdx].size = processSize;
            memory.insert(memory.begin() + bestIdx + 1, newBlock);
        }
        return true;
    }
    return false;
}

bool worstFit(vector<MemoryBlock>& memory, int processSize, const string& processName) {
    int worstIdx = -1;
    for (int i = 0; i < memory.size(); i++) {
        if (!memory[i].allocated && memory[i].size >= processSize) {
            if (worstIdx == -1 || memory[i].size > memory[worstIdx].size) {
                worstIdx = i;
            }
        }
    }

    if (worstIdx != -1) {
        memory[worstIdx].allocated = true;
        memory[worstIdx].process = processName;
        if (memory[worstIdx].size > processSize) {
            MemoryBlock newBlock = {
                memory[worstIdx].blockId,                // same blockId
                memory[worstIdx].size - processSize,
                false,
                ""
            };
            memory[worstIdx].size = processSize;
            memory.insert(memory.begin() + worstIdx + 1, newBlock);
        }
        return true;
    }
    return false;
}

bool nextFit(vector<MemoryBlock>& memory, int processSize, const string& processName, int& lastAllocated) {
    int start = (lastAllocated + 1) % memory.size();
    for (int i = 0; i < memory.size(); i++) {
        int idx = (start + i) % memory.size();
        if (!memory[idx].allocated && memory[idx].size >= processSize) {
            memory[idx].allocated = true;
            memory[idx].process = processName;
            if (memory[idx].size > processSize) {
                MemoryBlock newBlock = {
                    memory[idx].blockId,                  // same blockId
                    memory[idx].size - processSize,
                    false,
                    ""
                };
                memory[idx].size = processSize;
                memory.insert(memory.begin() + idx + 1, newBlock);
            }
            lastAllocated = idx;
            return true;
        }
    }
    return false;
}

void applyAllocation(vector<MemoryBlock>& memory, const vector<pair<int, string>>& processes, 
                    int method, int& lastAllocated) {
    cout << "\nApplying ";
    switch(method) {
        case 1: cout << "First Fit"; break;
        case 2: cout << "Best Fit"; break;
        case 3: cout << "Next Fit"; break;
        case 4: cout << "Worst Fit"; break;
    }
    cout << " allocation:\n";
    
    for (const auto& process : processes) {
        bool success;
        switch(method) {
            case 1: 
                success = firstFit(memory, process.first, process.second);
                break;
            case 2:
                success = bestFit(memory, process.first, process.second);
                break;
            case 3:
                success = nextFit(memory, process.first, process.second, lastAllocated);
                break;
            case 4:
                success = worstFit(memory, process.first, process.second);
                break;
        }
        
        cout << "Process " << process.second << " (" << process.first << "KB): "
             << (success ? "Allocated" : "Failed to allocate") << endl;
    }
    
    printMemory(memory);
}

int main() {
    vector<MemoryBlock> originalMemory;
    vector<pair<int, string>> processes;
    int lastAllocated = -1;
    
    // Input memory blocks
    cout << "Enter number of memory blocks: ";
    int numBlocks;
    cin >> numBlocks;
    
    for (int i = 0; i < numBlocks; i++) {
        cout << "Enter size of block " << i << " (in KB): ";
        int size;
        cin >> size;
        originalMemory.push_back({i, size, false, ""});  
    }
    
    // Input processes
    cout << "\nEnter number of processes: ";
    int numProcesses;
    cin >> numProcesses;
    
    for (int i = 0; i < numProcesses; i++) {
        cout << "Enter size of process " << i << " (in KB): ";
        int size;
        cin >> size;
        processes.push_back({size, "P" + to_string(i)});
    }
    
    // Create working memory
    vector<MemoryBlock> workingMemory;
    
    while (true) {
        cout << "\nMemory Allocation Techniques:\n";
        cout << "1. First Fit\n";
        cout << "2. Best Fit\n";
        cout << "3. Next Fit\n";
        cout << "4. Worst Fit\n";
        cout << "5. Compare All Methods\n";
        cout << "0. Exit\n";
        cout << "Choose option: ";
        
        int choice;
        cin >> choice;
        
        if (choice == 0) break;
        
        // Reset memory for each new allocation
        workingMemory = originalMemory;
        lastAllocated = -1;
        
        switch(choice) {
            case 1: case 2: case 3: case 4:
                applyAllocation(workingMemory, processes, choice, lastAllocated);
                break;
            case 5:
                // Compare all methods
                for (int method = 1; method <= 4; method++) {
                    workingMemory = originalMemory;
                    lastAllocated = -1;
                    applyAllocation(workingMemory, processes, method, lastAllocated);
                }
                break;
            default:
                cout << "Invalid choice!\n";
        }
    }
    
    return 0;
}
