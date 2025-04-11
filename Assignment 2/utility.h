#ifndef UTILITY_H
#define UTILITY_H

#include <vector>
#include <string>
#include "process.h"

void calculateTimes(std::vector<Process>& processes);
void displayResults(const std::vector<Process>& processes);
void printReadyQueue(const std::vector<int>& ready_queue, int current_time);
void displayGanttChart(const std::vector<GanttRecord>& gantt_chart);

#endif