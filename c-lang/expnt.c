#include <stdio.h>

int
expnt(int b, int n)
{
	static int a = 1;

	if (n == 0) {
		return a;
	} else if (n % 2 == 0) {
		expnt(b*b, n/2);
	} else {
		a = a * b;
		expnt(b, n-1);
	}
}

int
main(void)
{
	printf("%d\n", expnt(4, 3));
}
