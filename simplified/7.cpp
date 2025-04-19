#include<bits/stdc++.h>
using namespace std;

// FIFO using queue
void fifoPageReplacement(vector<int>& pages, int frameSize) {
    queue<int> q;
    unordered_set<int> s;
    int faults = 0;

    for (int p : pages) {
        if (s.find(p) == s.end()) {
            if (s.size() == frameSize) {
                s.erase(q.front());
                q.pop();
            }
            s.insert(p);
            q.push(p);
            faults++;
        }
    }
    cout << "FIFO Page Faults: " << faults << endl;
}

// LRU using list
void lruPageReplacement(vector<int>& pages, int frameSize) {
    list<int> l;
    unordered_set<int> s;
    int faults = 0;

    for (int p : pages) {
        if (s.find(p) == s.end()) {
            if (s.size() == frameSize) {
                s.erase(l.back());
                l.pop_back();
            }
            s.insert(p);
            l.push_front(p);
            faults++;
        } else {
            l.remove(p);
            l.push_front(p);
        }
    }
    cout << "LRU Page Faults: " << faults << endl;
}

// Optimal without queue
void optimalPageReplacement(vector<int>& pages, int frameSize) {
    vector<int> frames;
    int faults = 0, n = pages.size();

    for (int i = 0; i < n; i++) {
        int p = pages[i];
        if (find(frames.begin(), frames.end(), p) == frames.end()) {
            if (frames.size() < frameSize) frames.push_back(p);
            else {
                int farIdx = -1, idx = -1;
                for (int j = 0; j < frameSize; j++) {
                    int k;
                    for (k = i+1; k < n; k++)
                        if (pages[k] == frames[j]) break;
                    if (k > farIdx) farIdx = k, idx = j;
                }
                frames[idx] = p;
            }
            faults++;
        }
    }
    cout << "Optimal Page Faults: " << faults << endl;
}

int main() {
    vector<int> pages = {7,0,1,2,0,3,0,4,2,3,0,3,2};
    int frameSize = 3;

    fifoPageReplacement(pages, frameSize);
    lruPageReplacement(pages, frameSize);
    optimalPageReplacement(pages, frameSize);

    return 0;
}