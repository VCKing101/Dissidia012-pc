#include "main/auto_region2.h"

/* Allocator callback: frees *slot to the world heap and clears it. */
void func_088312EC(void *self, void **slot)
{
    if (*slot != 0) {
        func_08842138(*slot);
        *slot = 0;
    }
}
