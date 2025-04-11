#include <algorithm>
#include <iostream>
#include "priority.h"
#include "utility.h"
#include <climits>

void priority_non_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue) {
    std::vector<Process> temp = processes;
    int current_time = 0;
    int completed = 0;
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
        
        int start_time = current_time;
        current_time += temp[highest_priority].remaining_time;
        int end_time = current_time;
        
        // Add to Gantt chart
        gantt_chart.push_back(GanttRecord(temp[highest_priority].pid, start_time, end_time));
        
        temp[highest_priority].completion_time = current_time;
        temp[highest_priority].remaining_time = 0;
        temp[highest_priority].execution_intervals.push_back(std::make_pair(start_time, end_time));
        completed++;
    }
    
    processes = temp;
    calculateTimes(processes);
}

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
