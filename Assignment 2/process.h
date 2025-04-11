#ifndef PROCESS_H
#define PROCESS_H

#include <vector>

struct Process {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
    std::vector<std::pair<int, int>> execution_intervals; // Stores (start_time, end_time) for Gantt chart
    
    Process(int id, int at, int bt, int pr) {
        pid = id;
        arrival_time = at;
        burst_time = bt;
        priority = pr;
        remaining_time = bt;
        completion_time = 0;
        turnaround_time = 0;
        waiting_time = 0;
    }
};

struct GanttRecord {
    int pid;
    int start_time;
    int end_time;
    
    GanttRecord(int id, int st, int et) {
        pid = id;
        start_time = st;
        end_time = et;
    }
};

#endif
