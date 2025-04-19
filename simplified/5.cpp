#include<bits/stdc++.h>
using namespace std;

void bankersAlgorithm() {
    int n = 5, m = 3;  // processes, resources

    vector<vector<int>> alloc = {
        {0, 1, 0},
        {2, 0, 0},
        {3, 0, 2},
        {2, 1, 1},
        {0, 0, 2}
    };

    vector<vector<int>> maxNeed = {
        {7, 5, 3},
        {3, 2, 2},
        {9, 0, 2},
        {2, 2, 2},
        {4, 3, 3}
    };

    vector<int> avail = {3, 3, 2};

    vector<int> finish(n, 0), safeSeq;
    vector<vector<int>> need(n, vector<int>(m));

    for (int i = 0; i < n; i++) 
        for (int j = 0; j < m; j++) 
            need[i][j] = maxNeed[i][j] - alloc[i][j];

    int count = 0;
    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                int j;
                for (j = 0; j < m; j++)
                    if (need[i][j] > avail[j]) break;

                if (j == m) {
                    for (int k = 0; k < m; k++)
                        avail[k] += alloc[i][k];
                    safeSeq.push_back(i);
                    finish[i] = 1;
                    found = true;
                    count++;
                }
            }
        }
        if (!found) break;
    }

    if (count == n) {
        cout << "System is in a safe state.\nSafe Sequence: ";
        for (int p : safeSeq) cout << "P" << p << " ";
        cout << endl;
    } else {
        cout << "System is not in a safe state." << endl;
    }
}

int main() {
    bankersAlgorithm();
    return 0;
}