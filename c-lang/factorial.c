#include <stdio.h>

int
factorial(int n)
{
	int prod = 1;
	for (int i = 1; i <= n; i++)
		prod = prod * i;

	return prod;
}

int
main(void)
{
	printf("%d\n", factorial(6));
	return 1;
}
