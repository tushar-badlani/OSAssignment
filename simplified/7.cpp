#include <bits/stdc++.h>

using namespace std;


void lru(vector<int> pages, int frames){
    unordered_map<int,int> used;
    vector<int> memory;
    int fault=0;

    for(int i=0; i<pages.size(); i++){
        cout << "Accessing page: " << pages[i] << endl;
        int page = pages[i];

        if(find(memory.begin(), memory.end(), page) == memory.end()){
            fault++;
            if(memory.size()< frames){
                memory.push_back(page);
                used[page] = i;
                cout << "Inserted " << page << endl;
            }
            else{
                int min = INT_MAX, mini=-1;
                for(auto it: used){
                    if(it.second < min && find(memory.begin(), memory.end(), it.first) != memory.end()){
                        min = it.second;
                        mini = it.first;
                    }
                }
                replace(memory.begin(), memory.end(), mini, page);
                used[page] = i;
                cout << "Replaced " << mini << " with " << page << endl;
            }
        }
        else{
            used[page] = i;
            cout << "Page Hit" << endl;
        }
        cout << endl;
    }
    
    cout << "Total Page Faults (LRU): " << fault << endl;
    cout << "Memory: [ ";
    for (int page : memory) {
        cout << page << " ";
    }
    cout << "]" << endl;
    cout << "----------------------\n";

    cout << endl << endl;

}


void fifo(vector<int> pages, int frames){
    unordered_map<int,int> used;
    vector<int> memory;
    int fault=0;

    for(int i=0; i<pages.size(); i++){
        int page = pages[i];
        cout << "Accessing page: " << page << endl;

        if(find(memory.begin(), memory.end(), page) == memory.end()){
            fault++;

            if(memory.size()< frames){
                memory.push_back(page);
                used[page] = i;
                cout << "Inserted " << page << endl;
            }
            else{
                int min = INT_MAX, mini=-1;
                for(auto it: used){
                    if(it.second < min && find(memory.begin(), memory.end(), it.first) != memory.end()){
                        min = it.second;
                        mini = it.first;
                    }
                }
                replace(memory.begin(), memory.end(), mini, page);
                used[page] = i;
                cout << "Replaced " << mini << " with " << page << endl;
            }
        }
        else{
            cout << "Page Hit" << endl;
        }
        cout << endl;

    }
    cout << "Total Page Faults (FIFO): " << fault << endl;
    cout << "Memory: [ ";
    for (int page : memory) {
        cout << page << " ";
    }
    cout << "]" << endl;
    cout << "----------------------\n";
    cout << endl << endl;
}

void optimal(vector<int> pages, int frames){
    vector<int> memory;
    int fault =0;

    for(int i=0; i<pages.size(); i++){
        int page = pages[i];
        cout << "Accessing page: " << page << endl;

        if(find(memory.begin(), memory.end(), page)== memory.end()){
            fault++;
            

            if(memory.size() < frames){
                memory.push_back(page);
                cout << "Inserted " << page << endl;
            }
            else{
                int replace = -1, max = -1; 
                for(int j=0; j<frames; j++){
                    int far = -1;
                    for(int k=i; k<pages.size(); k++){
                        if(memory[j]==pages[k]){
                            far = k;
                            break;
                        }
                    }
                    if(far == -1){
                        replace = j;
                        max = -1;
                        break;
                    }
                    else{
                        if(far > max){
                            max = far;
                            replace = j;
                        }
                    }
                }

                memory[replace] = page;
                cout << "Replaced " << memory[replace] << " with " << page << endl;
                
            }
        }
        else{
            cout << "Page Hit" << endl;
        }
        cout << endl;
    }
    cout << "Total Page Faults (Optimal): " << fault << endl;
    cout << "Memory: [ ";
    for (int page : memory) {
        cout << page << " ";
    }
    cout << "]" << endl;
    cout << "----------------------\n";
    cout << endl << endl;
}

int main(){
    vector<int> pages = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3};
    int frames = 4;

    cout << "FIFO: ";
    fifo(pages, frames);

    cout << "LRU: ";
    lru(pages, frames);

    cout << "Optimal: ";
    optimal(pages, frames);

    return 0;
}
