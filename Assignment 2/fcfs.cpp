#include <algorithm>
#include <iostream>
#include "fcfs.h"
#include "utility.h"

void fcfs(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue) {
    std::sort(processes.begin(), processes.end(), 
             [](const Process& a, const Process& b) { return a.arrival_time < b.arrival_time; });
    
    int current_time = 0;
    std::vector<int> ready_queue;
    gantt_chart.clear();
    
    for(size_t i = 0; i < processes.size(); i++) {
        // Update ready queue
        ready_queue.clear();
        for(size_t j = i+1; j < processes.size() && processes[j].arrival_time <= current_time; j++) {
            ready_queue.push_back(processes[j].pid);
        }
        
        if(display_ready_queue) {
            printReadyQueue(ready_queue, current_time);
        }
        
        // If current process hasn't arrived yet, update time
        if(current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        int start_time = current_time;
        current_time += processes[i].burst_time;
        int end_time = current_time;
        
        // Add to Gantt chart
        gantt_chart.push_back(GanttRecord(processes[i].pid, start_time, end_time));
        
        // Update process completion time
        processes[i].completion_time = current_time;
        processes[i].execution_intervals.push_back(std::make_pair(start_time, end_time));
    }
    
    calculateTimes(processes);
}