#include <queue>
#include <iostream>
#include "round_robin.h"
#include "utility.h"
#include <climits>
#include <algorithm>


void round_robin(std::vector<Process>& processes, int quantum, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue) {
    std::vector<Process> temp = processes;
    std::queue<int> rq;
    std::vector<int> ready_queue;
    int current_time = 0;
    int completed = 0;
    gantt_chart.clear();
    
    // Sort by arrival time
    std::sort(temp.begin(), temp.end(), 
             [](const Process& a, const Process& b) { return a.arrival_time < b.arrival_time; });
    
    // Add first process to ready queue
    rq.push(0);
    int current_index = 1;
    
    while(completed != processes.size()) {
        if(rq.empty()) {
            // Find next process to arrive
            int min_arrival = INT_MAX;
            int next_process = -1;
            for(size_t i = 0; i < temp.size(); i++) {
                if(temp[i].remaining_time > 0 && temp[i].arrival_time < min_arrival) {
                    min_arrival = temp[i].arrival_time;
                    next_process = i;
                }
            }
            
            if(next_process != -1) {
                current_time = temp[next_process].arrival_time;
                rq.push(next_process);
                
                // Add any other processes that may have arrived at the same time
                for(size_t i = 0; i < temp.size(); i++) {
                    if(i != next_process && temp[i].arrival_time <= current_time && temp[i].remaining_time > 0) {
                        rq.push(i);
                    }
                }
            }
            continue;
        }
        
        int current_process = rq.front();
        rq.pop();
        
        // Create ready queue for display
        if(display_ready_queue) {
            ready_queue.clear();
            std::queue<int> temp_queue = rq;
            while(!temp_queue.empty()) {
                ready_queue.push_back(temp[temp_queue.front()].pid);
                temp_queue.pop();
            }
            printReadyQueue(ready_queue, current_time);
        }
        
        int execution_time = std::min(quantum, temp[current_process].remaining_time);
        int start_time = current_time;
        current_time += execution_time;
        int end_time = current_time;
        
        // Add to Gantt chart
        gantt_chart.push_back(GanttRecord(temp[current_process].pid, start_time, end_time));
        
        // Update remaining time
        temp[current_process].remaining_time -= execution_time;
        
        // Record execution interval
        temp[current_process].execution_intervals.push_back(std::make_pair(start_time, end_time));
        
        // Add newly arrived processes to ready queue
        for(size_t i = 0; i < temp.size(); i++) {
            if(temp[i].arrival_time > start_time && temp[i].arrival_time <= end_time && 
               temp[i].remaining_time > 0) {
                rq.push(i);
            }
        }
        
        if(temp[current_process].remaining_time > 0) {
            rq.push(current_process);
        } else {
            temp[current_process].completion_time = current_time;
            completed++;
        }
    }
    
    processes = temp;
    calculateTimes(processes);
}