#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void 
waitFor (unsigned int secs) {
    unsigned int retTime = time(0) + secs;   // Get finishing time.
    while (time(0) < retTime);               // Loop until it arrives.
}

int
mint(int *A, int len)
{
	int min = A[0];

	for (int i = 0; i<len; i++) {
		min = A[i] < min ? A[i] : min;
	}

	return min;
}

int
maxt(int *A, int len)
{
	int max = A[0];

	for (int i = 0; i<len; i++) {
		max = max < A[i] ? A[i] : max;
	}

	return max;
}

void
shit_sort(int *A, int len)
{
	int *x = malloc(len * sizeof(int));
	int max = maxt(A,len);
	int min = mint(A,len);

	for (int i = 0; i<len; i++) {
		x[i] = (rand() % (max+1-min)) + min;
	}

	for (int i = 0; i<len; i++) {
		printf("%d ",x[i]);
	}
	printf("\n");

	int check_sort = 1;
	for (int n = 0; n<len; n++) {
		if (n < len-1) {
			check_sort = x[n+1] < x[n] ? 0 : check_sort;
		}
	}

	if (check_sort == 0) {
		shit_sort(A,len);
		free(x);
		return;
	}

	int *checker = malloc(len * sizeof(int));
	for (int i = 0; i<len; i++) {
		checker[i] = 0;
	}

	for (int k = 0; k<len; k++) {
		int item = A[k];
		for (int j = 0; j<len; j++)  {
			if (x[j] == item && checker[j] == 0) {
				checker[j] = 1;
				break;
			}
		}
	}

	if (mint(checker,len) == 0) {
		shit_sort(A,len);
		free(x);
		free(checker);
		return;
	}

	for (int i = 0; i<len; i++) {
		A[i] = x[i];
	}

	return;
}

int
main(void)
{
	int Test[6] = {2,5,4,1,2,5};
	for (int i = 0; i<6; i++) {
		printf("%d ",Test[i]);
	}
	printf("\n");
	shit_sort(Test,6);
	for (int i = 0; i<6; i++) {
		printf("%d ",Test[i]);
	}
	printf("\n");

	return 0;
}
