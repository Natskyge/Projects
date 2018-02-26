#include "mergesort.h"
#include "kalender.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

void
merge(Task *A, int mid, int high, bool (*order)(Task, Task))
{
	/* Init temp array */
	Task *x = malloc(high * sizeof(Task));
	
	/* h pointer from middel in A, m from bottom in A, k places in x */
	int m,h,k;
	for (h = mid, m = 0, k = 0; k < high; ++k) {
		x[k] = m >= mid          ? A[h++]  /* m has reached mid    */
			 : h >= high         ? A[m++]  /* h has reached high   */
			 : order(A[h],A[m])  ? A[h++]  /* h < m, so we take h  */
			 : 				       A[m++]; /* h >= m, so we take m */
	}

	/* Dump x into A */
	for (int i = 0; i < high; i++) {
		A[i] = x[i];
	}

	free(x);
}

void
mergeSort(Task *A, int len, bool (*order)(Task, Task))
{
	/* Base case: 1 or 0 elements are always sorted */
	if (len < 2)
		return;

	int mid = len/2;

	/* sort left half and right half then merge */
	mergeSort(A,mid,order);
	mergeSort(A+mid,len-mid,order);
	merge(A,mid,len,order);
}
