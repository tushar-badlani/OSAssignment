#ifndef SJF_H
#define SJF_H

#include <vector>
#include "process.h"

void sjf_non_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue);
void sjf_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue);

#endif