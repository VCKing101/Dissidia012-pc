#include "menu/auto_region2.h"

/* Counts the nodes of a chain (func_08A42E14 steps), at least 1. */
int func_08A42EA4(unsigned short *node)
{
    int count;

    count = 0;
    while (node != 0) {
        node = func_08A42E14(node);
        count = count + 1;
    }
    if (count < 1) {
        count = 1;
    }
    return count;
}
