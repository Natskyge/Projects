#include <stdio.h>

float celsius_to_fahrenheit(float celsius)
{
    return (9.0/5) * celsius + 32.0;
}

int main(void)
{
    float celsius;
    int upper, lower, step;
    upper = 300;
    lower = 0;
    step = 10;
    celsius = lower;

    while(celsius <= upper)
    {
        printf("%3.0f %6.0f\n", celsius, celsius_to_fahrenheit(celsius));
        celsius = celsius + step;
    }
    return 0;
}
