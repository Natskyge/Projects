#include <stdio.h>
#include <math.h>


static long double delta;

static long double f(long double x);

static long double
f(long double x)
{
	return powl(x, 2) - 2;
}

static long double
differntial(long double x)
{
	return (f(x + delta) - f(x))/delta;
}

int
main(void)
{
	delta =  powl(10, -100);
	printf("%10.80Lf\n", differntial(2));
}
