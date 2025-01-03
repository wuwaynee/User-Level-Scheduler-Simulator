#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/task.h"
#include "../include/signal.h"
#include "../include/scheduler.h"
#include "../include/resource.h"

void create_task(char *task_name, void (*function_name)(), int priority)
{
    struct task *new_task = (struct task *)malloc(sizeof(struct task));
    new_task->task_name = (char *)malloc(10 * sizeof(char));

    getcontext(&new_task->ctx);
    new_task->ctx.uc_stack.ss_sp = malloc(CONTEXT_STACK);
    new_task->ctx.uc_stack.ss_size = CONTEXT_STACK;
    new_task->ctx.uc_link = 0;
    makecontext(&new_task->ctx, function_name, 0);

    new_task->TID = TID++;
    strcpy(new_task->task_name, task_name);
    new_task->task_state_t = READY;
    new_task->running_time = 0;
    new_task->waiting_time = 0;
    new_task->ready_time = 0;
    new_task->sleep_time = 0;
    new_task->num_resource_occupied = 0;
    for (int i = 0; i < NUM_RESOURCE; ++i)
        new_task->resource_occupied[i] = false;
    new_task->num_resource_waiting = 0;
    new_task->resource_waiting = NULL;
    new_task->priority = priority;

    if (!head)
        head = new_task;
    if (tail)
        tail->next_id = new_task;
    tail = new_task;
    new_task->next_id = NULL;

    insert_ready_queue(new_task);
}

struct task *find_task(char *task_name)
{
    struct task *temp = head;
    while (temp) {
        if (strcmp(task_name, temp->task_name) == 0)
            break;
        temp = temp->next_id;
    }
    return temp;
}

void print_task(struct task *target)
{
    fprintf(stdout," %3d| %10s| ", target->TID, target->task_name);
    switch (target->task_state_t) {
        case READY:
            fprintf(stdout, "%10s| ", "READY");
            break;
        case WAITING:
            fprintf(stdout, "%10s| ", "WAITING");
            break;
        case RUNNING:
            fprintf(stdout, "%10s| ", "RUNNING");
            break;
        case TERMINATED:
            fprintf(stdout, "%10s| ", "TERMINATED");
            break;
    }
    fprintf(stdout,"%7d| %7d| ", target->running_time, target->ready_time);
    if (target->task_state_t == TERMINATED)
        fprintf(stdout,"%10d| ", target->running_time + target->ready_time + target->waiting_time);
    else
        fprintf(stdout,"%10s| ", "none");
    if (target->num_resource_occupied == 0) {
        fprintf(stdout,"%9s", "none");
    } else {
        char temp[9];
        for (int i = 0; i < 9; ++i)
            temp[i] = ' ';
        
        int index = 8;
        for (int i = NUM_RESOURCE - 1; i >= 0; --i) {
            if (target->resource_occupied[i]) {
                char t[2];
                sprintf(t, "%d", i);
                temp[index] = t[0];
                index -= 2;
            }
        }
        fprintf(stdout,"%9s", temp);
    }
    if (scheduler == &priority_preemptive)
        fprintf(stdout, "| %8d", target->priority);
    fprintf(stdout, "\n");
}

void remove_from_queue(struct task *target, struct task **queue)
{
    struct task **temp = queue;
    while (*temp) {
        if (*temp == target) {
            *queue = target->next;
            if (target == ready_queue_tail)
                ready_queue_tail = *queue;
            break;
        }
        if ((*temp)->next == target) {
            (*temp)->next = target->next;
            if (target == ready_queue_tail)
                ready_queue_tail = (*temp);
            break;
        }
        temp = &(*temp)->next;
    }
}

void insert_ready_queue(struct task *ptr)
{
    if (scheduler == &priority_preemptive) {
        if (!ready_queue) {
            ready_queue = ready_queue_tail = ptr;
            ptr->next = NULL;
        } else if (ready_queue->priority > ptr->priority) {
            ptr->next = ready_queue;
            ready_queue = ptr;
        } else {
            struct task *temp = ready_queue;
            while (temp->next) {
                if (temp->next->priority < ptr->priority)
                    temp = temp->next;
                else
                    break;
            }
            ptr->next = temp->next;
            temp->next = ptr;
            if (ready_queue_tail == temp)
                ready_queue_tail = ptr;
        }
    } else {
        if (ready_queue_tail)
            ready_queue_tail->next = ptr;
        else
            ready_queue = ptr;
        ready_queue_tail = ptr;
        ptr->next = NULL;
    }
    ptr->task_state_t = READY;
}

void task_sleep(int ms)
{
    running_task->task_state_t = WAITING;
    running_task->sleep_time = ms;
    fprintf(stdout, "Task %s goes to sleep.\n", running_task->task_name);
    fflush(stdout);
}

void task_exit()
{
    while (running_task->task_state_t != RUNNING);
    running_task->task_state_t = TERMINATED;
    fprintf(stdout, "Task %s has terminated.\n", running_task->task_name);
    fflush(stdout);
}
