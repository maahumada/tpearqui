#ifndef __SCHEDULER_H__
#define __SCHEDULER_H__

#include <stdint.h>
#include <process.h>

void addSchedulerProcess(Process* process);
void removeSchedulerProcess(Process* process);

#endif