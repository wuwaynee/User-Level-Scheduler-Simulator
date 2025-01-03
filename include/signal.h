#ifndef SIGNAL_H
#define SIGNAL_H

#include <sys/time.h>
#include "task.h"

#define TIMER_SEC   0
#define TIMER_USEC  10000
#define RR_TQ 3

struct itimerval timer;
int time_quantum_remain;

void start_timer();
void stop_timer();
void reset_timer();
void calculate_waiting_time(struct task *);

void clock_handler();
void pause_handler();

#endif
