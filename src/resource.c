#include <stdio.h>
#include "../include/resource.h"
#include "../include/task.h"

bool is_resources_available(int count, int *resources)
{
    for (int i = 0; i < count; ++i)
        if (!resource_available[resources[i]])
            return false;
    return true;
}

void get_resources(int count, int *resources)
{
    while (1) {
        while (running_task->task_state_t != RUNNING);
        pthread_mutex_lock(&mutex);
        if (is_resources_available(count, resources))
            break;
        pthread_mutex_unlock(&mutex);
            
        running_task->num_resource_waiting = count;
        running_task->resource_waiting = resources;
        running_task->task_state_t = WAITING;

        fprintf(stdout, "Task %s is waiting resource.\n", running_task->task_name);
        fflush(stdout);
    }
    for (int i = 0; i < count; ++i) {
        resource_available[resources[i]] = false;
        running_task->resource_occupied[resources[i]] = true;
        running_task->num_resource_occupied++;
        fprintf(stdout, "Task %s gets resource %d.\n", running_task->task_name, resources[i]);
        fflush(stdout);
    }
    pthread_mutex_unlock(&mutex);
}

void release_resources(int count, int *resources)
{
    while (running_task->task_state_t != RUNNING);
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < count; ++i) {
        resource_available[resources[i]] = true;
        running_task->resource_occupied[resources[i]] = false;
        running_task->num_resource_occupied--;
        fprintf(stdout, "Task %s releases resource %d.\n", running_task->task_name, resources[i]);
        fflush(stdout);
    }
    pthread_mutex_unlock(&mutex);
}
