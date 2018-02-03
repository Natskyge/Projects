#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int
printArray(int array[], int len)
{
	for (int i = 0; i < len; ++i) {
		printf("%d ", array[i]);
	}
	printf("\n");
	return 0;
}

void quicksort(int *A, int len) {
	if (len < 2) return;

	int pivot = A[len / 2];

	int i, j;
	for (i = 0, j = len - 1; ; i++, j--) {
		while (A[i] < pivot) i++;
		while (A[j] > pivot) j--;

		if (i >= j) break;

		int temp = A[i];
		A[i]     = A[j];
		A[j]     = temp;
	}

	quicksort(A, i);
	quicksort(A + i, len - i);
}

// cond ? then : else

void
merge(int *A, int mid, int high)
{
	int *x = malloc(high * sizeof(int));
	
	int m,h,k;
	for (h = mid, m = 0, k = 0; k < high; ++k) {
		x[k] = m >= mid     ? A[h++]
			 : h >= high    ? A[m++]
			 : A[h] < A[m]  ? A[h++]
			 : 				  A[m++];
	}

	for (int i = 0; i < high; i++) {
		A[i] = x[i];
	}

	free(x);
}

void
mergeSort(int *A, int len)
{
	/* Base case: 1 or 0 elements are always sorted */
	if (len < 2)
		return;

	int mid = len/2;

	mergeSort(A,mid);
	mergeSort(A+mid,len-mid);
	merge(A,mid,len);
}

void
bubbleSort(int *A, int len)
{
	if (len < 2)
		return;

	int max = len;

	while (1 < max) {
		for (int i = 1; i < max; i++) {
			if (A[i] < A[i-1]) {
				int temp = A[i];
				A[i] = A[i-1];
				A[i-1] = temp;
			}
		}

		--max;
	}
}

int
main(void)
{
	int LEN = 100000;
	int test[LEN];
	for (int i = 0; i<LEN; i++) {
		test[i] = rand() % LEN;
	}
	//bubbleSort(test,LEN);
	//mergeSort(test,LEN);
	quicksort(test,LEN);
	return 0;
}
