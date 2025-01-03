#include <stdio.h>
#include <sys/time.h>
#include "../include/signal.h"
#include "../include/task.h"
#include "../include/scheduler.h"

void start_timer()
{
    setitimer(ITIMER_VIRTUAL, &timer, 0);
}

void stop_timer()
{
    struct itimerval temp;
    getitimer(ITIMER_VIRTUAL, &temp);
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer, 0);
    timer.it_value.tv_sec = temp.it_value.tv_sec;
    timer.it_value.tv_usec = temp.it_value.tv_usec;
}

void reset_timer()
{
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 0;
    setitimer(ITIMER_VIRTUAL, &timer, 0);
    timer.it_value.tv_sec = TIMER_SEC;
    timer.it_value.tv_usec = TIMER_USEC;
}

void calculate_waiting_time(struct task *head)
{
    while (head) {
        head->waiting_time++;
        head = head->next;
    }
}

void clock_handler()
{
    struct task *head;

    head = ready_queue;
    while (head) {
        head->ready_time++;
        head = head->next;
    }

    head = waiting_resource_queue;
    while (head) {
        head->waiting_time++;
        struct task *temp = head->next;
        if (is_resources_available(head->num_resource_waiting, head->resource_waiting)) {
            remove_from_queue(head, &waiting_resource_queue);
            insert_ready_queue(head);
            running_task->num_resource_waiting = 0;
            running_task->resource_waiting = NULL;
            preempt_flag = 1;
        }
        head = temp;
    }

    head = waiting_sleep_queue;
    while (head) {
        head->waiting_time++;
        head->sleep_time--;
        struct task *temp = head->next;
        if (head->sleep_time == 0) {
            remove_from_queue(head, &waiting_sleep_queue);
            insert_ready_queue(head);
            preempt_flag = 1;
        }
        head = temp;
    }

    if (running_task != idle_task) {
        struct task *temp = running_task;
        running_task->running_time++;
        if (running_task->task_state_t == TERMINATED) {
            running_task->next = terminated_queue;
            terminated_queue = running_task;
            running_task = NULL;
            current = &scheduler_ctx;
            swapcontext(&temp->ctx, &scheduler_ctx);
        } else if (running_task->task_state_t == WAITING) {
            if (running_task->sleep_time != 0) {
                running_task->next = waiting_sleep_queue;
                waiting_sleep_queue = running_task;
            } else {
                running_task->next = waiting_resource_queue;
                waiting_resource_queue = running_task;
            }
            running_task = NULL;
            current = &scheduler_ctx;
            swapcontext(&temp->ctx, &scheduler_ctx);
        }
        if (scheduler == &RR) {
            --time_quantum_remain;
            if (time_quantum_remain == 0) {
                insert_ready_queue(running_task);
                running_task = NULL;
                current = &scheduler_ctx;
                swapcontext(&temp->ctx, &scheduler_ctx);
            }
        }
        if (scheduler == &priority_preemptive && preempt_flag) {
            insert_ready_queue(running_task);
            running_task = NULL;
            current = &scheduler_ctx;
            swapcontext(&temp->ctx, &scheduler_ctx);
        }
    } else {
        if (ready_queue) {
            current = &scheduler_ctx;
            swapcontext(&idle_task->ctx, &scheduler_ctx);
        }
    }
}

void pause_handler()
{
    stop_timer();
    if (current == &shell_ctx)
        return;
    if (current == &scheduler_ctx) {
        return;
        swapcontext(&scheduler_ctx, &shell_ctx);
    } else {
        swapcontext(&running_task->ctx, &shell_ctx);
    }
}
