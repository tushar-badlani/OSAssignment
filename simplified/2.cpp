#include<bits/stdc++.h>
using namespace std;

class Process {
public:
    int id, arrival, burst, remaining, priority, completion, tat, wt;
    Process(int i=0, int a=0, int b=0, int p=0) {
        id = i;
        arrival = a;
        burst = b;
        remaining = b;
        priority = p;
        completion = tat = wt = 0;
    }
};

class CompareSJF {
public:
    bool operator()(Process a, Process b) {
        return a.burst > b.burst;
    }
};

class CompareFCFS{
    public:
    bool operator()(Process a, Process b) {
        return a.arrival > b.arrival;
    }
};

class CompareSJF_Preempt {
public:
    bool operator()(Process a, Process b) {
        return a.remaining > b.remaining;
    }
};

class ComparePriority {
public:
    bool operator()(Process a, Process b) {
        return a.priority > b.priority;
    }
};

void printTable(vector<Process> p) {
    cout << "PID  AT  BT  CT  TAT  WT\n";
    for(auto proc : p)
        cout << proc.id << "\t" << proc.arrival << "\t" << proc.burst << "\t" 
             << proc.completion << "\t" << proc.tat << "\t" << proc.wt << "\n";
}

void fcfs(vector<Process> p) {
    cout << "\n--- FCFS (with priority_queue) ---\n";
    int time = 0, n = p.size();
    double total_wt=0, total_tat=0;

    priority_queue<Process, vector<Process>, CompareFCFS> pq;
    for (auto proc : p) pq.push(proc);

    while (!pq.empty()) {
        Process proc = pq.top(); pq.pop();
        if (time < proc.arrival) time = proc.arrival;
        proc.wt = time - proc.arrival;
        proc.completion = time + proc.burst;
        proc.tat = proc.completion - proc.arrival;
        time += proc.burst;

        for (auto &pr : p)
            if (pr.id == proc.id) {
                pr.wt = proc.wt;
                pr.completion = proc.completion;
                pr.tat = proc.tat;
            }

        total_wt += proc.wt;
        total_tat += proc.tat;
    }

    printTable(p);
    cout << "Avg WT: " << total_wt/n << ", Avg TAT: " << total_tat/n << "\n";
}

void sjf_non_preemptive(vector<Process> p) {
    cout << "\n--- SJF Non-Preemptive ---\n";
    int n=p.size(), time=0, completed=0;
    double total_wt=0, total_tat=0;
    priority_queue<Process, vector<Process>, CompareSJF> pq;
    
    while(completed<n) {
        for(int i=0; i<n; i++){
            if(p[i].arrival >= time){
                pq.push(p[i]);
            }
        }

        if(!pq.empty()) {
            Process proc = pq.top(); pq.pop();
            proc.wt = time - proc.arrival;
            proc.completion = time + proc.burst;
            proc.tat = proc.completion - proc.arrival;
            time += proc.burst;
            for(auto pr : p)
                if(pr.id == proc.id) {
                    pr.wt = proc.wt;
                    pr.completion = proc.completion;
                    pr.tat = proc.tat;
                }
            total_wt += proc.wt;
            total_tat += proc.tat;
            completed++;
        } else time++;
    }
    printTable(p);
    cout << "Avg WT: " << total_wt/n << ", Avg TAT: " << total_tat/n << "\n";
}

void sjf_preemptive(vector<Process> p) {
    cout << "\n--- SJF Preemptive ---\n";
    int n=p.size(), completed=0, time=0;
    double total_wt=0, total_tat=0;
    priority_queue<Process, vector<Process>, CompareSJF_Preempt> pq;

    while(completed < n) {
        for(int i=0; i<n; i++){
            if(p[i].arrival == time){
                pq.push(p[i]);
            }
        }

        if(!pq.empty()) {
            Process proc = pq.top(); pq.pop();
            proc.remaining--;
            if(proc.remaining == 0) {
                proc.completion = time+1;
                proc.tat = proc.completion - proc.arrival;
                proc.wt = proc.tat - proc.burst;
                for(auto pr : p)
                    if(pr.id == proc.id) {
                        pr.completion = proc.completion;
                        pr.tat = proc.tat;
                        pr.wt = proc.wt;
                    }
                total_wt += proc.wt;
                total_tat += proc.tat;
                completed++;
            } else pq.push(proc);
        }
        time++;
    }
    printTable(p);
    cout << "Avg WT: " << total_wt/n << ", Avg TAT: " << total_tat/n << "\n";
}

void priority_non_preemptive(vector<Process> p) {
    cout << "\n--- Priority Non-Preemptive ---\n";
    int n=p.size(), time=0, completed=0;
    double total_wt=0, total_tat=0;
    priority_queue<Process, vector<Process>, ComparePriority> pq;
    

    while(completed<n) {
        for(int i=0; i<n; i++){
            if(p[i].arrival >= time){
                pq.push(p[i]);
            }
        }  

        if(!pq.empty()) {
            Process proc = pq.top(); 
            pq.pop();
            if(time < proc.arrival) time = proc.arrival;
            proc.wt = time - proc.arrival;
            proc.completion = time + proc.burst;
            proc.tat = proc.completion - proc.arrival;
            time += proc.burst;
            for(auto pr : p)
                if(pr.id == proc.id) {
                    pr.wt = proc.wt;
                    pr.completion = proc.completion;
                    pr.tat = proc.tat;
                }
            total_wt += proc.wt;
            total_tat += proc.tat;
            completed++;
        } else time++;
    }
    printTable(p);
    cout << "Avg WT: " << total_wt/n << ", Avg TAT: " << total_tat/n << "\n";
}

void priority_preemptive(vector<Process> p) {
    cout << "\n--- Priority Preemptive ---\n";
    int n=p.size(), completed=0, time=0;
    double total_wt=0, total_tat=0;
    priority_queue<Process, vector<Process>, ComparePriority> pq;

    while(completed<n) {
        for(int i=0; i<n; i++){
            if(p[i].arrival == time){
                pq.push(p[i]);
            }
        }

        if(!pq.empty()) {
            Process proc = pq.top(); 
            pq.pop();
            proc.remaining--;
            if(proc.remaining==0) {
                proc.completion = time+1;
                proc.tat = proc.completion - proc.arrival;
                proc.wt = proc.tat - proc.burst;
                for(auto pr : p)
                    if(pr.id==proc.id) {
                        pr.completion = proc.completion;
                        pr.tat = proc.tat;
                        pr.wt = proc.wt;
                    }
                total_wt+=proc.wt;
                total_tat+=proc.tat;
                completed++;
            } else pq.push(proc);
        }
        time++;
    }
    printTable(p);
    cout << "Avg WT: " << total_wt/n << ", Avg TAT: " << total_tat/n << "\n";
}

void round_robin(vector<Process> p, int tq) {
    cout << "\n--- Round Robin ---\n";
    int n=p.size(), completed=0, time=0;
    double total_wt=0, total_tat=0;
    queue<Process> q;
    
    while(completed < n) {
        for(int i=0; i<n; i++){
            if(p[i].arrival == time){
                q.push(p[i]);
            }
        }

        if(!q.empty()) {
            Process proc = q.front(); 
            q.pop();
            int exec = min(tq, proc.remaining);
            proc.remaining -= exec;
            time += exec;
            for(int i=0; i<n; i++){
                if(p[i].arrival == time){
                    q.push(p[i]);
                }
            }

            if(proc.remaining==0) {
                proc.completion = time;
                proc.tat = proc.completion - proc.arrival;
                proc.wt = proc.tat - proc.burst;
                for(auto pr : p)
                    if(pr.id==proc.id) {
                        pr.completion = proc.completion;
                        pr.tat = proc.tat;
                        pr.wt = proc.wt;
                    }
                total_wt+=proc.wt;
                total_tat+=proc.tat;
                completed++;
            } else q.push(proc);
        } else time++;
    }
    printTable(p);
    cout << "Avg WT: " << total_wt/n << ", Avg TAT: " << total_tat/n << "\n";
}

int main() {
    vector<Process> processes = {
        Process(1,0,5,2), Process(2,1,3,1), Process(3,2,8,3), Process(4,3,6,0)
    };

    fcfs(processes);
    sjf_non_preemptive(processes);
    sjf_preemptive(processes);
    priority_non_preemptive(processes);
    priority_preemptive(processes);
    round_robin(processes, 2);

    return 0;
}
