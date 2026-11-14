#include "main/auto_region2.h"

/* func_0888A2B4(0, 1) when first is set, then func_0888A2B4(1, 0). */
void func_0888A3A4(unsigned char first)
{
    int i;

    if (first != '\0') {
        func_0888A2B4(0, 1);
    }
    i = 1;
    do {
        func_0888A2B4(i, 0);
        i = i + 1;
    } while (i < 2);
}
