#include <stdio.h>
#include <ucontext.h>
#include "../include/task.h"
#include "../include/signal.h"
#include "../include/scheduler.h"

void FCFS()
{
    while (1) {
        running_task = ready_queue;
        if (ready_queue)
            ready_queue = ready_queue->next;
        if (!ready_queue)
            ready_queue_tail = NULL;

        if (!running_task) {
            if (!waiting_resource_queue && !waiting_sleep_queue) {
                fprintf(stdout, "Simulation over.\n");
                fflush(stdout);
                reset_timer();
                swapcontext(&scheduler_ctx, &shell_ctx);
            } else {
                fprintf(stdout, "CPU idle.\n");
                fflush(stdout);
                running_task = idle_task;
                current = NULL;
                swapcontext(&scheduler_ctx, &idle_task->ctx);
            }
        } else {
            running_task->next = NULL;
            running_task->task_state_t = RUNNING;
            fprintf(stdout, "Task %s is running.\n", running_task->task_name);
            fflush(stdout);
            current = NULL;
            swapcontext(&scheduler_ctx, &running_task->ctx);
        }
    }
}

void RR()
{
    while (1) {
        time_quantum_remain = RR_TQ;
        
        running_task = ready_queue;
        if (ready_queue)
            ready_queue = ready_queue->next;
        if (!ready_queue)
            ready_queue_tail = NULL;

        if (!running_task) {
            if (!waiting_resource_queue && !waiting_sleep_queue) {
                fprintf(stdout, "Simulation over.\n");
                fflush(stdout);
                reset_timer();
                swapcontext(&scheduler_ctx, &shell_ctx);
            } else {
                fprintf(stdout, "CPU idle.\n");
                fflush(stdout);
                running_task = idle_task;
                current = NULL;
                swapcontext(&scheduler_ctx, &idle_task->ctx);
            }
        } else {
            running_task->next = NULL;
            running_task->task_state_t = RUNNING;
            fprintf(stdout, "Task %s is running.\n", running_task->task_name);
            fflush(stdout);
            current = NULL;
            swapcontext(&scheduler_ctx, &running_task->ctx);
        }
    }
}

void priority_preemptive()
{
    while (1) {
        preempt_flag = 0;
        running_task = ready_queue;
        if (ready_queue)
            ready_queue = ready_queue->next;
        if (!ready_queue)
            ready_queue_tail = NULL;

        if (!running_task) {
            if (!waiting_resource_queue && !waiting_sleep_queue) {
                fprintf(stdout, "Simulation over.\n");
                fflush(stdout);
                reset_timer();
                swapcontext(&scheduler_ctx, &shell_ctx);
            } else {
                fprintf(stdout, "CPU idle.\n");
                fflush(stdout);
                running_task = idle_task;
                current = NULL;
                swapcontext(&scheduler_ctx, &idle_task->ctx);
            }
        } else {
            running_task->next = NULL;
            running_task->task_state_t = RUNNING;
            fprintf(stdout, "Task %s is running.\n", running_task->task_name);
            fflush(stdout);
            current = NULL;
            swapcontext(&scheduler_ctx, &running_task->ctx);
        }
    }
}
