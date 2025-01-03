#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ucontext.h>
#include <signal.h>
#include "include/shell.h"
#include "include/command.h"
#include "include/task.h"
#include "include/scheduler.h"
#include "include/signal.h"
#include "include/function.h"
#include "include/resource.h"

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Please determine the scheduling algorithm.\n");
		return 0;
	}
	if (strcmp(argv[1], "FCFS") == 0)
		scheduler = &FCFS;
	else if (strcmp(argv[1], "RR") == 0)
		scheduler = &RR;
	else
		scheduler = &priority_preemptive;
	preempt_flag = 0;

	signal(SIGVTALRM, clock_handler);
	signal(SIGTSTP, pause_handler);

	history_count = 0;
	for (int i = 0; i < MAX_RECORD_NUM; ++i)
    	history[i] = (char *)malloc(BUF_SIZE * sizeof(char));

	timer.it_interval.tv_sec = TIMER_SEC;
	timer.it_interval.tv_usec = TIMER_USEC;
	timer.it_value.tv_sec = TIMER_SEC;
	timer.it_value.tv_usec = TIMER_USEC;

	time_quantum_remain = RR_TQ;

	TID = 1;
	head = NULL;
	tail = NULL;
	ready_queue = NULL;
	ready_queue_tail = NULL;
	running_task = NULL;
	waiting_resource_queue = NULL;
	waiting_sleep_queue = NULL;
	terminated_queue = NULL;

	for (int i = 0; i < NUM_RESOURCE; ++i)
		resource_available[i] = true;

	idle_task = (struct task *)malloc(sizeof(struct task));
	getcontext(&idle_task->ctx);
    idle_task->ctx.uc_stack.ss_sp = malloc(CONTEXT_STACK);
    idle_task->ctx.uc_stack.ss_size = CONTEXT_STACK;
    idle_task->ctx.uc_link = 0;
    makecontext(&idle_task->ctx, idle, 0);

	getcontext(&scheduler_ctx);
    scheduler_ctx.uc_stack.ss_sp = malloc(CONTEXT_STACK);
    scheduler_ctx.uc_stack.ss_size = CONTEXT_STACK;
    scheduler_ctx.uc_link = 0;
    makecontext(&scheduler_ctx, scheduler, 0);

	getcontext(&shell_ctx);
    shell_ctx.uc_stack.ss_sp = malloc(CONTEXT_STACK);
    shell_ctx.uc_stack.ss_size = CONTEXT_STACK;
    shell_ctx.uc_link = 0;
    makecontext(&shell_ctx, shell, 0);

	swapcontext(&main_ctx, &shell_ctx);

	free(shell_ctx.uc_stack.ss_sp);
	free(scheduler_ctx.uc_stack.ss_sp);
	free(idle_task->ctx.uc_stack.ss_sp);
	free(idle_task);
	while (terminated_queue) {
		struct task *temp = terminated_queue;
		terminated_queue = terminated_queue->next;
		free(temp->task_name);
		free(temp->ctx.uc_stack.ss_sp);
		free(temp);
	}
	for (int i = 0; i < MAX_RECORD_NUM; ++i)
    	free(history[i]);

	return 0;
}
