#include "main/auto_region2.h"

/* Counts the items func_088350AC walks for kind. */
int func_08835284(ActorView *self, int kind)
{
    void *item;
    int count;

    count = 0;
    item = 0;
    while (item = func_088350AC(self, kind, item), item != 0) {
        count = count + 1;
    }
    return count;
}
