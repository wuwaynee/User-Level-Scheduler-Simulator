#ifndef SCHEDULER_H
#define SCHEDULER_H

int preempt_flag;

void (*scheduler)();

void FCFS();
void RR();
void priority_preemptive();

#endif
