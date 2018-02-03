#include <stdio.h>

int
factorial(int n)
{
	static int a = 1;

	if (n == 0)
		return a;

	a *= n;
	factorial(n-1);
}

int
main(void)
{
	printf("%d\n", factorial(6));
	return 0;
}
