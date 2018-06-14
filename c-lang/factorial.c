#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int
factorial(int n)
{
	static int acc = 1;

	if (n == 0) {
		return acc;
	} else {
		acc *= n;
		return factorial(n-1);
	}
}

int
expnt_helper(int acc, int b, int n)
{
	if ( n == 0 ) {
		return acc;
	} else if ( n % 2 == 0 ) {
		return expnt_helper(acc, b*b, n/2);
	} else {
		return expnt_helper(acc*b,b,n-1);
	}
}

int
expnt(int b, int n)
{
	return expnt_helper(1,b,n);
}

int
main(void)
{
	printf("%d\n", expnt(2,3));
	return 0;
}
