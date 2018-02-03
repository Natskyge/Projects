#include <stdio.h>
#include <stdbool.h>

int
mod(int a, int b)
{
   if(b < 0)
     return mod(a, -b);   

   int ret = a % b;

   if(ret < 0)
     ret+=b;

   return ret;
}

bool
isPrime(int n)
{
	static int d = 2;

	while (n >= d*d) {
		if (mod(n,d) == 0)
			return false;

		++d;
	}

	return true;
}

void
primeFactor(int n)
{
	static int q = 2;
	
	if (isPrime(n)) {
		printf("%d", n);
	} else if (0 == mod(n, q)) {
		printf("%d*", q);
		q = 2;
		primeFactor(n/q);
	} else {
		++q;
		primeFactor(n);
	}
}

int
main(void)
{
	primeFactor(1001);
	printf("\n");
	return 0;
}
