#include<bits/stdc++.h>
using namespace std;

void firstFit(vector<int> blocks, vector<int> processes) {
    int n = blocks.size(), m = processes.size();
    vector<int> alloc(m, -1);

    for (int i = 0; i < m; i++)
        for (int j = 0; j < n; j++)
            if (blocks[j] >= processes[i]) {
                alloc[i] = j;
                blocks[j] -= processes[i];
                break;
            }

    cout << "\nFirst Fit Allocation:\n";
    int frag = 0;
    for (int i = 0; i < m; i++) {
        cout << "Process " << i+1 << " -> ";
        if (alloc[i] != -1) cout << "Block " << alloc[i]+1 << endl;
        else cout << "Not Allocated\n";
    }
    frag = accumulate(blocks.begin(), blocks.end(), 0);
    cout << "Total Fragmentation: " << frag << endl;
}

void bestFit(vector<int> blocks, vector<int> processes) {
    int n = blocks.size(), m = processes.size();
    vector<int> alloc(m, -1);

    for (int i = 0; i < m; i++) {
        int bestIdx = -1;
        for (int j = 0; j < n; j++)
            if (blocks[j] >= processes[i])
                if (bestIdx == -1 || blocks[j] < blocks[bestIdx])
                    bestIdx = j;
        if (bestIdx != -1) {
            alloc[i] = bestIdx;
            blocks[bestIdx] -= processes[i];
        }
    }

    cout << "\nBest Fit Allocation:\n";
    int frag = 0;
    for (int i = 0; i < m; i++) {
        cout << "Process " << i+1 << " -> ";
        if (alloc[i] != -1) cout << "Block " << alloc[i]+1 << endl;
        else cout << "Not Allocated\n";
    }
    frag = accumulate(blocks.begin(), blocks.end(), 0);
    cout << "Total Fragmentation: " << frag << endl;
}

void worstFit(vector<int> blocks, vector<int> processes) {
    int n = blocks.size(), m = processes.size();
    vector<int> alloc(m, -1);

    for (int i = 0; i < m; i++) {
        int worstIdx = -1;
        for (int j = 0; j < n; j++)
            if (blocks[j] >= processes[i])
                if (worstIdx == -1 || blocks[j] > blocks[worstIdx])
                    worstIdx = j;
        if (worstIdx != -1) {
            alloc[i] = worstIdx;
            blocks[worstIdx] -= processes[i];
        }
    }

    cout << "\nWorst Fit Allocation:\n";
    int frag = 0;
    for (int i = 0; i < m; i++) {
        cout << "Process " << i+1 << " -> ";
        if (alloc[i] != -1) cout << "Block " << alloc[i]+1 << endl;
        else cout << "Not Allocated\n";
    }
    frag = accumulate(blocks.begin(), blocks.end(), 0);
    cout << "Total Fragmentation: " << frag << endl;
}

void nextFit(vector<int> blocks, vector<int> processes) {
    int n = blocks.size(), m = processes.size();
    vector<int> alloc(m, -1);
    int lastPos = 0;

    for (int i = 0; i < m; i++) {
        int j = lastPos, count = 0;
        while (count < n) {
            if (blocks[j] >= processes[i]) {
                alloc[i] = j;
                blocks[j] -= processes[i];
                lastPos = j;
                break;
            }
            j = (j + 1) % n;
            count++;
        }
    }

    cout << "\nNext Fit Allocation:\n";
    int frag = 0;
    for (int i = 0; i < m; i++) {
        cout << "Process " << i+1 << " -> ";
        if (alloc[i] != -1) cout << "Block " << alloc[i]+1 << endl;
        else cout << "Not Allocated\n";
    }
    frag = accumulate(blocks.begin(), blocks.end(), 0);
    cout << "Total Fragmentation: " << frag << endl;
}

int main() {
    vector<int> blocks = {100, 500, 200, 300, 600};
    vector<int> processes = {212, 417, 112, 426};

    firstFit(blocks, processes);
    bestFit(blocks, processes);
    worstFit(blocks, processes);
    nextFit(blocks, processes);

    return 0;
}
