#include<bits/stdc++.h>
using namespace std;

vector<int> fifo(vector<int> requests, int head, int seek_time){
    int current = head;
    vector<int> order;
    int total = 0;

    for(int i = 0; i < requests.size(); i++){
        order.push_back(requests[i]);
        total += abs(current - requests[i]);
        current = requests[i];
    }
    cout << "Total head movements: " << total << endl;
   cout << "Total seek time: " << total * seek_time << endl;
    return order;
}

vector<int> sstf(vector<int> requests, int head, int seek_time ){
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
   cout << "Total seek time: " << total * seek_time << endl;
    return order;
}

vector<int> scan(vector<int> requests, int head, int disk_size,int seek_time){
    vector<int> order;
    int total = 0;
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

    // Move to the end of disk if needed
    if (current != disk_size-1) {
        total += abs(current - (disk_size-1));
        current = disk_size-1;
    }

    // Then move to the left
    for(int i = requests.size()-1; i >= 0; i--){
        if(requests[i] < head){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements: " << total << endl;
 cout << "Total seek time: " << total * seek_time << endl;
    return order;
}

vector<int> cscan(vector<int> requests, int head, int disk_size,int seek_time){
    vector<int> order;
    int total = 0;
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

    // Jump to the beginning (0)
    if (current != disk_size-1) {
        total += abs(current - (disk_size-1));
        current = 0;
        total += (disk_size-1);
    } else {
        current = 0;
        total += (disk_size-1);
    }

    // Then move to the remaining on left side
    for(int i = 0; i < requests.size(); i++){
        if(requests[i] < head){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements: " << total << endl;
    cout << "Total seek time: " << total * seek_time << endl;
    return order;
}

vector<int> scan_left(vector<int> requests, int head, int disk_size,int seek_time){
    vector<int> order;
    int total = 0;
    int current = head;

    sort(requests.begin(), requests.end());

    // Move to the left first
    for(int i = requests.size()-1; i >= 0; i--){
        if(requests[i] <= current){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    // Move to start of disk if needed
    if (current != 0) {
        total += abs(current - 0);
        current = 0;
    }

    // Then move to the right
    for(int i = 0; i < requests.size(); i++){
        if(requests[i] > head){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements: " << total << endl;
    cout << "Total seek time: " << total * seek_time << endl;
    return order;
}

vector<int> cscan_left(vector<int> requests, int head, int disk_size, int seek_time){
    vector<int> order;
    int total = 0;
    int current = head;

    sort(requests.begin(), requests.end());

    // Move to the left first
    for(int i = requests.size()-1; i >= 0; i--){
        if(requests[i] <= current){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    // Jump to end of disk
    if (current != 0) {
        total += abs(current - 0);
        total += (disk_size-1);
        current = disk_size-1;
    }

    // Then move to remaining requests on the right side
    for(int i = requests.size()-1; i >= 0; i--){
        if(requests[i] > head){
            order.push_back(requests[i]);
            total += abs(current - requests[i]);
            current = requests[i];
        }
    }

    cout << "Total head movements: " << total << endl;
    
    cout << "Total seek time: " << total * seek_time << endl;
    return order;
}

int main(){
    
    int head=48, disk_size=100;
    vector<int> requests = {20,44,89,12,77,55,7,63,33,9};
    int seek_time = 5;

    cout << "\nFIFO Order: ";
    vector<int> fifo_order = fifo(requests, head ,seek_time);
    for(int i : fifo_order) cout << i << " ";
    cout << endl;

    cout << "\nSSTF Order: ";
    vector<int> sstf_order = sstf(requests, head,seek_time);
    for(int i : sstf_order) cout << i << " ";
    cout << endl;

    cout << "\nSCAN (Right) Order: ";
    vector<int> scan_order = scan(requests, head, disk_size,seek_time);
    for(int i : scan_order) cout << i << " ";
    cout << endl;

    cout << "\nCSCAN (Right) Order: ";
    vector<int> cscan_order = cscan(requests, head, disk_size,seek_time);
    for(int i : cscan_order) cout << i << " ";
    cout << endl;

    cout << "\nSCAN (Left) Order: ";
    vector<int> scan_left_order = scan_left(requests, head, disk_size,seek_time);
    for(int i : scan_left_order) cout << i << " ";
    cout << endl;

    cout << "\nCSCAN (Left) Order: ";
    vector<int> cscan_left_order = cscan_left(requests, head, disk_size,seek_time);
    for(int i : cscan_left_order) cout << i << " ";
    cout << endl;

    return 0;
}