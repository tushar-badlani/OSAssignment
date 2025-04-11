#include <iostream>
#include <iomanip>
#include "utility.h"

void calculateTimes(std::vector<Process>& processes) {
    for(auto& p : processes) {
        p.turnaround_time = p.completion_time - p.arrival_time;
        p.waiting_time = p.turnaround_time - p.burst_time;
    }
}

void displayResults(const std::vector<Process>& processes) {
    float avg_tat = 0, avg_wt = 0;
    
    std::cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for(const auto& p : processes) {
        std::cout << p.pid << "\t" << p.arrival_time << "\t" << p.burst_time << "\t"
                 << p.completion_time << "\t" << p.turnaround_time << "\t" << p.waiting_time << std::endl;
        avg_tat += p.turnaround_time;
        avg_wt += p.waiting_time;
    }
    
    avg_tat /= processes.size();
    avg_wt /= processes.size();
    std::cout << "\nAverage Turnaround Time: " << avg_tat;
    std::cout << "\nAverage Waiting Time: " << avg_wt << std::endl;
}

void printReadyQueue(const std::vector<int>& ready_queue, int current_time) {
    std::cout << "At time " << current_time << ", Ready Queue: [";
    for(size_t i = 0; i < ready_queue.size(); i++) {
        std::cout << "P" << ready_queue[i];
        if(i < ready_queue.size() - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
}

void displayGanttChart(const std::vector<GanttRecord>& gantt_chart) {
    if(gantt_chart.empty()) return;
    
    std::cout << "\nGantt Chart:\n";
    
    // Find the maximum end time to determine the chart width
    int max_end_time = 0;
    for(const auto& record : gantt_chart) {
        max_end_time = std::max(max_end_time, record.end_time);
    }
    
    // Top border
    std::cout << " ";
    for(const auto& record : gantt_chart) {
        int width = record.end_time - record.start_time;
        std::cout << std::string(width * 2 - 1, '-') << " ";
    }
    std::cout << "\n|";
    
    // Process IDs
    for(const auto& record : gantt_chart) {
        int width = record.end_time - record.start_time;
        std::cout << "P" << record.pid << std::string(width * 2 - 2, ' ') << "|";
    }
    std::cout << "\n ";
    
    // Bottom border
    for(const auto& record : gantt_chart) {
        int width = record.end_time - record.start_time;
        std::cout << std::string(width * 2 - 1, '-') << " ";
    }
    std::cout << "\n";
    
    // Timeline markers
    std::cout << "0";
    for(const auto& record : gantt_chart) {
        int width = record.end_time - record.start_time;
        std::cout << std::string(width * 2 - 1, ' ') << record.end_time;
    }
    std::cout << "\n\n";
    
    // Print execution details
    std::cout << "Process Execution Details:\n";
    for(const auto& record : gantt_chart) {
        std::cout << "P" << record.pid << ": ";
        std::cout << "Start Time = " << record.start_time << ", ";
        std::cout << "End Time = " << record.end_time << ", ";
        std::cout << "Duration = " << (record.end_time - record.start_time) << std::endl;
    }
}
