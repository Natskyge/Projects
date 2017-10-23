#include <stdio.h>

int main(void)
{
    float fahr, cel;
    int upper, lower, step;

    upper = 300;
    lower = 0;
    step = 10;
    fahr = lower;

    printf("Fahrenheit     Celsius\n");
    while(fahr <= upper)
    {
        cel = (5.0/9.0) * (fahr - 32.0);
        printf("%10.0f\t%6.1f\n", fahr, cel);
        fahr = fahr + step;
    }
}

/*
 * Note: Remember to specify number of digits after a dot for floats,
 *       also remember floats and ints are not the same.
 *       Syntax: %d, takes and int and prints one
 *               %nd, prints an n wide int
 *               %f, prints a float
 *               %nf, prints a n wide float
 *               %.kf, prints a float with k decimals
 *               %n.k, prints n wide float with k decimals
 */
