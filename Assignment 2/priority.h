#ifndef PRIORITY_H
#define PRIORITY_H

#include <vector>
#include "process.h"

void priority_non_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue);
void priority_preemptive(std::vector<Process>& processes, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue);

#endif
