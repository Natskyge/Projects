#include <stdio.h>
#include <stdlib.h>

int
main(void)
{
    for (int i; i < 10; i++) {
        printf("%d %d\n", abs(-i) % 8, i % 8);
    }
}
