#include "main/auto_region2.h"

/* The n-th item func_088350AC walks for kind (0 past the end). */
void *func_088352E8(ActorView *self, int kind, int n)
{
    void *item;
    int i;

    item = 0;
    i = 0;
    do {
        item = func_088350AC(self, kind, item);
        if (i == n) {
            return item;
        }
        i = i + 1;
    } while (item != 0);
    return 0;
}
