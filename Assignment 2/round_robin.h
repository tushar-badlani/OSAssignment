#ifndef ROUND_ROBIN_H
#define ROUND_ROBIN_H

#include <vector>
#include "process.h"

void round_robin(std::vector<Process>& processes, int quantum, std::vector<GanttRecord>& gantt_chart, bool display_ready_queue);

#endif
