/* Euler's number approximation program
 * Should work, but shits fucked, yo.
 */

#include <stdio.h>

static long factorial(int n);
static long double euler(int itterations);

long
factorial(int n)
{
    static int product = 1;
    for (int k = 1; k < n; k++) {
        product = (k + 1) * product;
    }
    return product;
}

long double
euler(int itterations)
{
    static long double sum = 0.0;
    for (int n = 0; n < itterations; n++) {
        printf("%Lf\n", sum);
        sum += 1.0 / factorial(n);
    }
    return sum;
}

int main(void)
{
    printf("%Lf\n", euler(12));
}
