#include<bits/stdc++.h>
using namespace std;


class Process {
    
    public:
    int id, at, bt, tat, ct, wt, rt, priority;
    Process(int id, int at, int bt, int priority){
        this -> id = id;
        this -> at = at;
        this -> bt = bt;
        this -> tat =0;
        this -> ct = 0;
        this -> wt =0;
        this -> rt = bt;
        this -> priority = priority;
    }

};


class compareSJF{
    public:
    bool operator()(Process a, Process b){
        return a.bt > b.bt;
    }
};

class compareFCFS{
    public:
    bool operator()(Process a, Process b){
        return a.at > b.at;
    }
};

class compareSJFPre{
    public:
    bool operator()(Process a, Process b){
        return a.rt > b.rt;
    }
};


class comparePriority{
    public:
    bool operator()(Process a, Process b){
        return a.priority > b.priority;
    }
};


void printTable(vector<Process> p){
    cout << "PID\tAT\tBT\tCT\tTAT\tWT\n";
    for(auto proc : p)
        cout << proc.id << "\t" << proc.at << "\t" << proc.bt << "\t" 
             << proc.ct << "\t" << proc.tat << "\t" << proc.wt << "\n";
}




void fcfs(vector<Process> p){
    int time =0, n = p.size();

    priority_queue<Process, vector<Process>, compareFCFS> pq;

    vector<Process> p1;

    int tat =0, wt =0;

    for(auto process : p){
        pq.push(process);
    }
    int completed =0;

    while(completed < n){
        Process pr = pq.top();
        pq.pop();

        if(pr.at > time){
            time = pr.at;
        }
        pr.ct = time+ pr.bt;
        pr.tat = pr.ct-pr.at;
        pr.wt = pr.tat-pr.bt;
        time+=pr.bt;
        completed++;

        p1.push_back(pr);
        tat += pr.tat;
        wt += pr.wt;
    }

    cout << tat << " " << wt << endl;
    printTable(p1);
    cout << "Average TAT: " << (float)tat/n << endl;
    cout << "Average WT: " << (float)wt/n << endl;
    cout << "----------------------\n";
    cout << endl << endl;

}


void sjfn(vector<Process> p){
    int time =0, n = p.size();

    priority_queue<Process, vector<Process>, compareSJF> pq;
    vector<Process> p1;

    int tat =0, wt =0;

    int completed = 0;
    vector<bool> pushed(n, false); 

    while(completed <n){
        for(int i=0; i<n; i++){
            if(p[i].at<=time && !pushed[i]){
                pq.push(p[i]);
                pushed[i] = true;
            }
        }
        if(!pq.empty()){
            Process pr = pq.top();
            pq.pop();
            if(pr.at > time){
                time = pr.at;
            }
            pr.ct = time+ pr.bt;
            pr.tat = pr.ct-pr.at;
            pr.wt = pr.tat-pr.bt;
            time+=pr.bt;
            completed++;
            p1.push_back(pr);
            tat += pr.tat;
            wt += pr.wt;

        }
        else{
            time++;
        }
    }
    cout << tat << " " << wt << endl;
    printTable(p1);
    cout << "Average TAT: " << (float)tat/n << endl;
    cout << "Average WT: " << (float)wt/n << endl;
    cout << "----------------------\n";
    cout << endl << endl;
}


void priorityn(vector<Process> p){
    int time =0, n = p.size();

    priority_queue<Process, vector<Process>, comparePriority> pq;
    vector<Process> p1;

    int tat =0, wt =0;

    int completed = 0;
    vector<bool> pushed(n, false); 

    while(completed <n){
        for(int i=0; i<n; i++){
            if(p[i].at<=time && !pushed[i]){
                pq.push(p[i]);
                pushed[i] = true;
            }
        }
        if(!pq.empty()){
            Process pr = pq.top();
            pq.pop();
            if(pr.at > time){
                time = pr.at;
            }
            pr.ct = time+ pr.bt;
            pr.tat = pr.ct-pr.at;
            pr.wt = pr.tat-pr.bt;
            time+=pr.bt;
            completed++;
            p1.push_back(pr);
            tat += pr.tat;
            wt += pr.wt;

        }
        else{
            time++;
        }
    }
    cout << tat << " " << wt << endl;
    printTable(p1);
    cout << "Average TAT: " << (float)tat/n << endl;
    cout << "Average WT: " << (float)wt/n << endl;
    cout << "----------------------\n";
    cout << endl << endl;
}


void sjfp(vector<Process> p){
    int time =0, n = p.size();

    priority_queue<Process, vector<Process>, compareSJFPre> pq;
    vector<Process> p1;

    int tat =0, wt =0;

    int completed = 0;
    vector<bool> pushed(n, false); 

    while(completed <n){
        for(int i=0; i<n; i++){
            if(p[i].at==time && !pushed[i]){
                pq.push(p[i]);
                pushed[i] = true;
            }
        }
        if(!pq.empty()){
            Process pr = pq.top();
            pq.pop();
            pr.rt--;
            time++;

            if(pr.rt ==0){
                pr.ct = time;
                pr.tat = pr.ct-pr.at;
                pr.wt = pr.tat-pr.bt;
                completed++;
                p1.push_back(pr);
                tat += pr.tat;
                wt += pr.wt;
            }

            else{
                pq.push(pr);
            }
        }
        else{
            time++;
        }
    }
    cout << tat << " " << wt << endl;
    printTable(p1);
    cout << "Average TAT: " << (float)tat/n << endl;
    cout << "Average WT: " << (float)wt/n << endl;
    cout << "----------------------\n";
    cout << endl << endl;
}


void priorityp(vector<Process> p){
    int time =0, n = p.size();

    priority_queue<Process, vector<Process>, comparePriority> pq;

    int tat =0, wt =0;
    vector<Process> p1;

    int completed = 0;
    vector<bool> pushed(n, false); 

    while(completed <n){
        for(int i=0; i<n; i++){
            if(p[i].at==time && !pushed[i]){
                pq.push(p[i]);
                pushed[i] = true;
            }
        }
        if(!pq.empty()){
            Process pr = pq.top();
            pq.pop();
            pr.rt--;
            time++;

            if(pr.rt ==0){
                pr.ct = time;
                pr.tat = pr.ct-pr.at;
                pr.wt = pr.tat-pr.bt;
                completed++;
                p1.push_back(pr);
                tat += pr.tat;
                wt += pr.wt;
            }

            else{
                pq.push(pr);
            }
        }
        else{
            time++;
        }
    }

    cout << tat << " " << wt << endl;
    printTable(p1);
    cout << "Average TAT: " << (float)tat/n << endl;
    cout << "Average WT: " << (float)wt/n << endl;
    cout << "----------------------\n";
    cout << endl << endl;
}


void rr(vector<Process> p, int tq){

    int n = p.size(), time =0, completed =0;
    vector<bool> pushed(n, false);
    queue<Process> q;
    vector<Process> p1;
    int tat =0, wt=0;

    while(completed < n){
        for(int i=0; i<n; i++){
            if(p[i].at <= time && !pushed[i]){
                q.push(p[i]);
                pushed[i] = true;
            }
        }
        if(!q.empty()){
            Process pr = q.front();
            q.pop();
            int et = min(tq, pr.rt);
            pr.rt -=et;
            time+=et;
            if(pr.rt ==0){
                pr.ct= time;
                pr.tat = time-pr.at;
                pr.wt = pr.tat-pr.bt;
                completed++;
                p1.push_back(pr);
                tat += pr.tat;
                wt+=pr.wt;
            }
            else{
                q.push(pr);
            }
        }
        else{
            time++;
        }
    }
    cout << tat << " " << wt << endl;
    printTable(p1);
    cout << "Average TAT: " << (float)tat/n << endl;
    cout << "Average WT: " << (float)wt/n << endl;
    cout << "----------------------\n";
    cout << endl << endl;
}


int main() {
    vector<Process> processes = {
        Process(1,0,5,2), Process(2,1,3,1), Process(3,2,8,3), Process(4,3,6,0)
    };

    cout << "FCFS:\n";
    fcfs(processes);
    cout << "SJF Non-Preemptive:\n";
    sjfn(processes);
    cout << "Priority Non-Preemptive:\n";
    priorityn(processes);
    cout << "SJF Pre-emptive:\n";
    sjfp(processes);
    cout << "Priority Pre-emptive:\n";
    priorityp(processes);
    cout << "Round Robin:\n";
    rr(processes, 3);
    

    return 0;
}