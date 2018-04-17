#ifndef KALENDER_H_
#define KALENDER_H_

#include "list.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Task {
	char *name;
	size_t v, v_i;
	size_t d, d_i;
	size_t t, t_i;
	size_t e, e_i;
} Task;

Task makeTask(char *name, size_t v, size_t d, size_t t, size_t e);
void addTask(Task task, List *taskList);
void getIndexs(List *taskList);
List* sortTasks(List *taskList);

#endif // KALENDER_H_
