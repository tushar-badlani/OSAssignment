#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <climits>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    std::vector<std::pair<int, int>> execution_intervals;
    
    Process(int id, int at, int bt, int pr) : 
        pid(id), arrival_time(at), burst_time(bt), priority(pr),
        remaining_time(bt), completion_time(0), turnaround_time(0), waiting_time(0) {}
};

struct GanttRecord {
    int pid;
    int start_time;
    int end_time;
    
    GanttRecord(int id, int st, int et) : 
        pid(id), start_time(st), end_time(et) {}
};

void calculateTimes(std::vector<Process>& processes) {
    for(auto& p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

void printReadyQueue(const std::vector<int>& ready_queue, int current_time) {
    std::cout << "Time " << current_time << ": [";
    for(size_t i = 0; i < ready_queue.size(); ++i) {
        std::cout << "P" << ready_queue[i];
        if(i < ready_queue.size() - 1) std::cout << ", ";
    }
    std::cout << "]\n";
}

void displayResults(const std::vector<Process>& processes) {
    float avg_tat = 0, avg_wt = 0;
    std::cout << "\nPID\tAT\tBT\tCT\tTAT\tWT\n";
    for(const auto& p : processes) {
        std::cout << p.pid << "\t"
                  << p.arrival_time << "\t"
                  << p.burst_time << "\t"
                  << p.completion_time << "\t"
                  << p.turnaround_time << "\t"
                  << p.waiting_time << "\n";
        avg_tat += p.turnaround_time;
        avg_wt += p.waiting_time;
    }
    std::cout << "\nAverage TAT: " << avg_tat/processes.size()
              << "\nAverage WT: " << avg_wt/processes.size() << "\n";
}

void displayGanttChart(const std::vector<GanttRecord>& gantt) {
    if(gantt.empty()) return;
    std::cout << "\nGantt Chart:\n";
    for(const auto& record : gantt) {
        std::cout << "| P" << record.pid << " (" 
                  << record.start_time << "-" << record.end_time << ") ";
    }
    std::cout << "|\n\n";
}

// Updated FCFS with per-second ready queue display
void fcfs(std::vector<Process>& processes, std::vector<GanttRecord>& gantt, bool show_rq) {
    std::sort(processes.begin(), processes.end(), 
        [](const Process& a, const Process& b) { return a.arrival_time < b.arrival_time; });
    
    int current_time = 0;
    size_t idx = 0;
    gantt.clear();

    while(idx < processes.size()) {
        // Handle idle time
        if(current_time < processes[idx].arrival_time) {
            for(int t = current_time; t < processes[idx].arrival_time; ++t) {
                if(show_rq) printReadyQueue({}, t);
            }
            current_time = processes[idx].arrival_time;
        }

        // Show ready queue at process start
        std::vector<int> rq;
        for(size_t i = idx+1; i < processes.size(); ++i) {
            if(processes[i].arrival_time <= current_time) 
                rq.push_back(processes[i].pid);
        }
        if(show_rq) printReadyQueue(rq, current_time);

        // Execute process
        int start = current_time;
        current_time += processes[idx].burst_time;
        gantt.emplace_back(processes[idx].pid, start, current_time);
        processes[idx].completion_time = current_time;

        // Update ready queue during execution
        for(int t = start+1; t < current_time; ++t) {
            std::vector<int> new_rq;
            for(size_t i = idx+1; i < processes.size(); ++i) {
                if(processes[i].arrival_time <= t) 
                    new_rq.push_back(processes[i].pid);
            }
            if(show_rq) printReadyQueue(new_rq, t);
        }

        idx++;
    }
    calculateTimes(processes);
}

// Updated SJF (Non-preemptive) with per-second ready queue
void sjf_non_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt, bool show_rq) {
    std::vector<Process> temp = processes;
    int current_time = 0;
    int completed = 0;
    gantt.clear();

    while(completed < temp.size()) {
        // Find shortest job
        int sj_idx = -1;
        int min_bt = INT_MAX;
        std::vector<int> rq;

        for(size_t i = 0; i < temp.size(); ++i) {
            if(temp[i].arrival_time <= current_time && temp[i].remaining_time > 0) {
                rq.push_back(temp[i].pid);
                if(temp[i].burst_time < min_bt) {
                    min_bt = temp[i].burst_time;
                    sj_idx = i;
                }
            }
        }

        if(sj_idx == -1) { // Idle
            if(show_rq) printReadyQueue({}, current_time);
            current_time++;
            continue;
        }

        if(show_rq) printReadyQueue(rq, current_time);

        // Execute process
        int start = current_time;
        current_time += temp[sj_idx].burst_time;
        gantt.emplace_back(temp[sj_idx].pid, start, current_time);
        temp[sj_idx].completion_time = current_time;
        temp[sj_idx].remaining_time = 0;
        completed++;

        // Show ready queue during execution
        for(int t = start+1; t < current_time; ++t) {
            std::vector<int> new_rq;
            for(const auto& p : temp) {
                if(p.arrival_time <= t && p.remaining_time > 0 && p.pid != temp[sj_idx].pid)
                    new_rq.push_back(p.pid);
            }
            if(show_rq) printReadyQueue(new_rq, t);
        }
    }
    processes = temp;
    calculateTimes(processes);
}

// Updated Round Robin with per-second ready queue
void round_robin(std::vector<Process>& processes, int quantum, std::vector<GanttRecord>& gantt, bool show_rq) {
    std::queue<size_t> rq;
    std::vector<Process> temp = processes;
    std::vector<bool> in_queue(temp.size(), false);
    int current_time = 0;
    size_t idx = 0;
    gantt.clear();

    // Sort by arrival time
    std::sort(temp.begin(), temp.end(),
        [](const Process& a, const Process& b) { return a.arrival_time < b.arrival_time; });

    while(idx < temp.size() || !rq.empty()) {
        // Add arriving processes
        while(idx < temp.size() && temp[idx].arrival_time <= current_time) {
            rq.push(idx);
            in_queue[idx] = true;
            idx++;
        }

        if(rq.empty()) { // Idle time
            if(show_rq) printReadyQueue({}, current_time);
            current_time++;
            continue;
        }

        size_t current_idx = rq.front();
        rq.pop();
        in_queue[current_idx] = false;

        // Show ready queue at start
        std::vector<int> current_rq;
        std::queue<size_t> copy = rq;
        while(!copy.empty()) {
            current_rq.push_back(temp[copy.front()].pid);
            copy.pop();
        }
        if(show_rq) printReadyQueue(current_rq, current_time);

        int exec_time = std::min(quantum, temp[current_idx].remaining_time);
        int end_time = current_time + exec_time;
        gantt.emplace_back(temp[current_idx].pid, current_time, end_time);

        // Process execution with per-second updates
        for(int t = current_time; t < end_time; ++t) {
            // Add new arrivals
            while(idx < temp.size() && temp[idx].arrival_time == t) {
                rq.push(idx);
                in_queue[idx] = true;
                idx++;
            }

            // Update remaining time
            temp[current_idx].remaining_time--;

            // Show ready queue
            if(t != current_time && show_rq) {
                std::vector<int> new_rq;
                std::queue<size_t> copy = rq;
                while(!copy.empty()) {
                    new_rq.push_back(temp[copy.front()].pid);
                    copy.pop();
                }
                printReadyQueue(new_rq, t);
            }
        }

        current_time = end_time;
        if(temp[current_idx].remaining_time > 0) {
            rq.push(current_idx);
            in_queue[current_idx] = true;
        } else {
            temp[current_idx].completion_time = current_time;
        }
    }
    processes = temp;
    calculateTimes(processes);
}

void sjf_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue) {
    std::vector<Process> temp = processes;
    int current_time = 0;
    int completed = 0;
    int prev_process = -1;
    int curr_start_time = 0;
    gantt_chart.clear();
    
    while(completed != processes.size()) {
        int shortest_job = -1;
        int min_burst = INT_MAX;
        
        // Create ready queue and find shortest job
        std::vector<int> ready_queue;
        for(size_t i = 0; i < temp.size(); i++) {
            if(temp[i].arrival_time <= current_time && temp[i].remaining_time > 0) {
                ready_queue.push_back(temp[i].pid);
                if(temp[i].remaining_time < min_burst) {
                    min_burst = temp[i].remaining_time;
                    shortest_job = i;
                }
            }
        }
        
        if(display_ready_queue) {
            printReadyQueue(ready_queue, current_time);
        }
        
        if(shortest_job == -1) {
            current_time++;
            continue;
        }
        
        // Context switch - create a new Gantt record
        if(prev_process != temp[shortest_job].pid && prev_process != -1) {
            gantt_chart.push_back(GanttRecord(prev_process, curr_start_time, current_time));
            curr_start_time = current_time;
        } else if(prev_process == -1) {
            curr_start_time = current_time;
        }
        
        prev_process = temp[shortest_job].pid;
        temp[shortest_job].remaining_time--;
        current_time++;
        
        if(temp[shortest_job].remaining_time == 0) {
            completed++;
            temp[shortest_job].completion_time = current_time;
            gantt_chart.push_back(GanttRecord(temp[shortest_job].pid, curr_start_time, current_time));
            prev_process = -1;
            
            // Record execution interval
            temp[shortest_job].execution_intervals.push_back(std::make_pair(curr_start_time, current_time));
        }
    }
    
    processes = temp;
    calculateTimes(processes);
}

void priority_non_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt, bool show_rq) {
    std::vector<Process> temp = processes;
    int current_time = 0;
    int completed = 0;
    gantt.clear();

    while(completed < temp.size()) {
        // Find highest priority process (lowest priority number)
        int hp_idx = -1;
        int min_priority = INT_MAX;
        std::vector<int> rq;

        // Build ready queue and find highest priority process
        for(size_t i = 0; i < temp.size(); ++i) {
            if(temp[i].arrival_time <= current_time && temp[i].remaining_time > 0) {
                rq.push_back(temp[i].pid);
                if(temp[i].priority < min_priority) {
                    min_priority = temp[i].priority;
                    hp_idx = i;
                }
            }
        }

        if(hp_idx == -1) { // No process available (idle time)
            if(show_rq) printReadyQueue({}, current_time);
            current_time++;
            continue;
        }

        // Show ready queue at selection time
        if(show_rq) printReadyQueue(rq, current_time);

        // Execute the entire process (non-preemptive)
        int start = current_time;
        current_time += temp[hp_idx].burst_time;
        gantt.emplace_back(temp[hp_idx].pid, start, current_time);
        temp[hp_idx].completion_time = current_time;
        temp[hp_idx].remaining_time = 0;
        completed++;

        // Show ready queue during execution (for each time unit)
        for(int t = start + 1; t < current_time; ++t) {
            std::vector<int> new_rq;
            for(size_t i = 0; i < temp.size(); ++i) {
                // Include processes that have arrived but aren't the current one
                if(i != hp_idx && temp[i].arrival_time <= t && temp[i].remaining_time > 0) {
                    new_rq.push_back(temp[i].pid);
                }
            }
            if(show_rq) printReadyQueue(new_rq, t);
        }
    }
    processes = temp;
    calculateTimes(processes);
}

// Priority Preemptive Algorithm
void priority_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue) {
    std::vector<Process> temp = processes;
    int current_time = 0;
    int completed = 0;
    int prev_process = -1;
    int curr_start_time = 0;
    gantt_chart.clear();
    
    while(completed != processes.size()) {
        int highest_priority = -1;
        int min_priority = INT_MAX;
        
        // Create ready queue and find highest priority job
        std::vector<int> ready_queue;
        for(size_t i = 0; i < temp.size(); i++) {
            if(temp[i].arrival_time <= current_time && temp[i].remaining_time > 0) {
                ready_queue.push_back(temp[i].pid);
                if(temp[i].priority < min_priority) {
                    min_priority = temp[i].priority;
                    highest_priority = i;
                }
            }
        }
        
        if(display_ready_queue) {
            printReadyQueue(ready_queue, current_time);
        }
        
        if(highest_priority == -1) {
            current_time++;
            continue;
        }
        
        // Context switch - create a new Gantt record
        if(prev_process != temp[highest_priority].pid && prev_process != -1) {
            gantt_chart.push_back(GanttRecord(prev_process, curr_start_time, current_time));
            curr_start_time = current_time;
        } else if(prev_process == -1) {
            curr_start_time = current_time;
        }
        
        prev_process = temp[highest_priority].pid;
        temp[highest_priority].remaining_time--;
        current_time++;
        
        if(temp[highest_priority].remaining_time == 0) {
            completed++;
            temp[highest_priority].completion_time = current_time;
            gantt_chart.push_back(GanttRecord(temp[highest_priority].pid, curr_start_time, current_time));
            prev_process = -1;
            
            // Record execution interval
            temp[highest_priority].execution_intervals.push_back(std::make_pair(curr_start_time, current_time));
        }
    }
    
    processes = temp;
    calculateTimes(processes);
}

// Main function
int main() {
    int n, choice;
    std::cout << "Enter number of processes: ";
    std::cin >> n;
    
    std::vector<Process> processes;
    
    // Input process details
    for(int i = 0; i < n; i++) {
        int at, bt, priority;
        std::cout << "\nProcess " << i + 1 << ":\n";
        std::cout << "Arrival Time: ";
        std::cin >> at;
        std::cout << "Burst Time: ";
        std::cin >> bt;
        std::cout << "Priority (lower number = higher priority): ";
        std::cin >> priority;
        processes.push_back(Process(i + 1, at, bt, priority));
    }
    
    bool display_ready_queue = true;
    std::vector<GanttRecord> gantt_chart;
    
    do {
        std::cout << "\nCPU Scheduling Algorithms\n";
        std::cout << "1. First Come First Serve (FCFS)\n";
        std::cout << "2. Shortest Job First (Non-preemptive)\n";
        std::cout << "3. Shortest Job First (Preemptive)\n";
        std::cout << "4. Round Robin\n";
        std::cout << "5. Priority (Non-preemptive)\n";
        std::cout << "6. Priority (Preemptive)\n";
        std::cout << "7. Toggle Ready Queue Display (Currently " << (display_ready_queue ? "ON" : "OFF") << ")\n";
        std::cout << "8. Exit\n";
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        
        std::vector<Process> temp = processes;
        
        switch(choice) {
            case 1:
                fcfs(temp, gantt_chart, display_ready_queue);
                std::cout << "\nFCFS Results:";
                displayResults(temp);
                displayGanttChart(gantt_chart);
                break;
                
            case 2:
                sjf_non_preemptive(temp, gantt_chart, display_ready_queue);
                std::cout << "\nNon-preemptive SJF Results:";
                displayResults(temp);
                displayGanttChart(gantt_chart);
                break;
                
            case 3:
                sjf_preemptive(temp, gantt_chart, display_ready_queue);
                std::cout << "\nPreemptive SJF Results:";
                displayResults(temp);
                displayGanttChart(gantt_chart);
                break;
                
            case 4:
                int quantum;
                std::cout << "Enter time quantum: ";
                std::cin >> quantum;
                round_robin(temp, quantum, gantt_chart, display_ready_queue);
                std::cout << "\nRound Robin Results:";
                displayResults(temp);
                displayGanttChart(gantt_chart);
                break;
                
            case 5:
                priority_non_preemptive(temp, gantt_chart, display_ready_queue);
                std::cout << "\nNon-preemptive Priority Results:";
                displayResults(temp);
                displayGanttChart(gantt_chart);
                break;
                
            case 6:
                priority_preemptive(temp, gantt_chart, display_ready_queue);
                std::cout << "\nPreemptive Priority Results:";
                displayResults(temp);
                displayGanttChart(gantt_chart);
                break;
                
            case 7:
                display_ready_queue = !display_ready_queue;
                std::cout << "Ready Queue Display is now " << (display_ready_queue ? "ON" : "OFF") << std::endl;
                break;
                
            case 8:
                std::cout << "Exiting...\n";
                break;
                
            default:
                std::cout << "Invalid choice!\n";
        }
    } while(choice != 8);
    
    return 0;
}