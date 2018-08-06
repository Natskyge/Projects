

/*
 * task.h - Struct for tasks
 *
 */

#ifndef TASK_H
#define TASK_H

#include <stdint.h>
#include <stdlib.h>
#include <time.h>

#ifndef MAX_HEIGHT
#define MAX_HEIGHT 16
#endif

typedef struct Task {
	char name[140];
	size_t importance, time_, energy;
	time_t deadline;
	int score;
} Task;

#endif /* TASK_H */
