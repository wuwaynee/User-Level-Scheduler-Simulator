#ifndef TASK_H
#define TASK_H

#include <stdbool.h>
#include <ucontext.h>
#include "resource.h"

#define CONTEXT_STACK 65536

int TID;

enum task_state {
    READY,
    RUNNING,
    WAITING,
    TERMINATED
};

struct task {
    int TID;
    char *task_name;
    enum task_state task_state_t;
    int running_time;
    int waiting_time;
    int ready_time;
    int sleep_time;
    int num_resource_occupied;
    bool resource_occupied[NUM_RESOURCE];
    int num_resource_waiting;
    int *resource_waiting;
    int priority;
    ucontext_t ctx;
    struct task *next_id;
    struct task *next;
};

struct task *head;
struct task *tail;
struct task *ready_queue;
struct task *ready_queue_tail;
struct task *running_task;
struct task *waiting_resource_queue;
struct task *waiting_sleep_queue;
struct task *terminated_queue;

struct task *idle_task;

ucontext_t main_ctx, shell_ctx, scheduler_ctx;
ucontext_t *current;

void create_task(char *task_name, void (*function_name)(), int priority);
struct task *find_task(char *task_name);
void print_task(struct task *);
void remove_from_queue(struct task *target, struct task **queue);
void insert_ready_queue(struct task *);

void task_sleep(int);
void task_exit();

#endif
