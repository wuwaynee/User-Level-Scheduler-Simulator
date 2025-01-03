#ifndef RESOURCE_H
#define RESOURCE_H

#include <pthread.h>
#include <stdbool.h>

#define NUM_RESOURCE 8

pthread_mutex_t mutex;

bool resource_available[NUM_RESOURCE];

bool is_resources_available(int, int *);
void get_resources(int, int *);
void release_resources(int, int *);

#endif
