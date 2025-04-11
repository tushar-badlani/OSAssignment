#include <iostream>
#include <vector>
#include "process.h"
#include "utility.h"
#include "fcfs.h"
#include "sjf.h"
#include "round_robin.h"
#include "priority.h"

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
