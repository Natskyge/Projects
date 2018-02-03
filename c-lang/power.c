#include <stdio.h>
#include <inttypes.h>

intmax_t
mul(intmax_t a, intmax_t b)
{
	if (0 == b) {
		return 0;
	} else if (b % 2) {
		return a + mul(a, b-1);
	} else {
		intmax_t temp = a << 1;
		return temp + mul(a, b-2);
	}
}

long
power(long base, long expnt)
{
	if (0 == expnt) {
		return 1;
	} else if (expnt % 2) {
		return base * power(base, expnt-1);
	} else {
		long tmp = power(base, expnt/2);
		return tmp * tmp;
	}
}

int
main(void)
{
	intmax_t a = 1000;
	intmax_t b = 1000;
	mul(a,b);
	return 0;
}
