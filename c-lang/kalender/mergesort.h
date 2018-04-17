#ifndef MERGESORT_H_
#define MERGESORT_H_

#include "list.h"
#include "kalender.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

/* NOTE: Order in the form < */
void merge(Task *A, int mid, int high, bool (*order)(Task, Task));
void mergeSort(Task *A, int len, bool (*order)(Task, Task));

#endif // MERGESORT_H_
