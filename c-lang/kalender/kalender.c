#include "kalender.h"
#include "list.h"
#include "mergesort.h"
#include <stdio.h>
#include <stdlib.h>

Task makeTask(char *name, size_t v, size_t d, size_t t, size_t e);
void addTask(Task task, List *taskList);
void getIndexs(List *taskList);
List* sortTasks(List *taskList);
