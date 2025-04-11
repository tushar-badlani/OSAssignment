#ifndef FCFS_H
#define FCFS_H

#include <vector>
#include "process.h"

void fcfs(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue);

#endif