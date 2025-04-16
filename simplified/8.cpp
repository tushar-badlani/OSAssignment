#include<bits/stdc++.h>
using namespace std;


vector<int> fifo(vector<int> requests, int head){
    int current = head;
    vector<int> order;
    int total = 0;

    for(int i = 0; i < requests.size(); i++){
        order.push_back(requests[i]);
        total += abs(current - requests[i]);
        current = requests[i];
    }
    cout << "Total head movements: " << total << endl;
    return order;
}


vector<int> sstf(vector<int> requests, int head){
    vector<int> order;
    int total = 0;
    vector<bool> visited(requests.size(), false);
    int current = head;

    
    while(order.size() < requests.size()){
        int min_dist = INT_MAX;
        int min_index = -1;

        for(int i = 0; i < requests.size(); i++){
            if(!visited[i]){
                int dist = abs(current - requests[i]);
                if(dist < min_dist){
                    min_dist = dist;
                    min_index = i;
                }
            }
        }

        visited[min_index] = true;
        order.push_back(requests[min_index]);
        total += min_dist;
        current = requests[min_index];
    }
    cout << "Total head movements: " << total << endl;
    return order;

}

vector<int> scan(vector<int> requests, int head){
    vector<int> order;
    int total = 0;
    vector<bool> visited(requests.size(), false);
    int current = head;

    sort(requests.begin(), requests.end());

    // Move to the right first
    for(int i = 0; i < requests.size(); i++){
        if(requests[i] >= current){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    // Then move to the left
    for(int i = requests.size() - 1; i >= 0; i--){
        if(requests[i] < current){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }
    cout << "Total head movements: " << total << endl;
    return order;
}

vector<int> cscan(vector<int> requests, int head){
    vector<int> order;
    int total = 0;
    vector<bool> visited(requests.size(), false);
    int current = head;

    sort(requests.begin(), requests.end());

    // Move to the right first
    for(int i = 0; i < requests.size(); i++){
        if(requests[i] >= current){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    // Jump to the beginning
    if(!requests.empty()){
        total += abs(current - requests[0]);
        current = requests[0];
    }

    // Then move to the left
    for(int i = 0; i < requests.size(); i++){
        if(requests[i] > current){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }
    cout << "Total head movements: " << total << endl;
    return order;
}


int main(){
    vector<int> requests = {98, 183, 37, 122, 14, 124, 65, 67};
    int head = 53;

    cout << "FIFO Order: ";
    vector<int> fifo_order = fifo(requests, head);
    for(int i : fifo_order) cout << i << " ";
    cout << endl;

    cout << "SSTF Order: ";
    vector<int> sstf_order = sstf(requests, head);
    for(int i : sstf_order) cout << i << " ";
    cout << endl;

    cout << "SCAN Order: ";
    vector<int> scan_order = scan(requests, head);
    for(int i : scan_order) cout << i << " ";
    cout << endl;

    cout << "CSCAN Order: ";
    vector<int> cscan_order = cscan(requests, head);
    for(int i : cscan_order) cout << i << " ";
    cout << endl;

}