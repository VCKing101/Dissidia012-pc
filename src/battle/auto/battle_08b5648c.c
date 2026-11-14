#include "battle/auto_region5.h"

/* Picks one of the ids 0x29..0x38 that func_0889877C accepts, at random
 * (a linear congruential step on *seed), and hands it to func_0881BAF8. */
void func_08B5648C(int *seed)
{
    void *object;
    int next;
    int id;
    int count;

    count = 0;
    for (id = 0x29; id < 0x39; id++) {
        object = func_08899300();
        if (func_0889877C(object, id) != 0) {
            count = count + 1;
        }
    }
    next = *seed * 0x343FD + 0x269EC3;
    count = (int)(next >> 0x10 & 0x7FFFU) % count;
    *seed = next;
    for (id = 0x29; id < 0x39; id++) {
        object = func_08899300();
        if (func_0889877C(object, id) != 0) {
            if (count == 0) {
                object = func_0881BAA8();
                func_0881BAF8(object, id, 0, 0, 1, 1000, 1);
                return;
            }
            count = count + -1;
        }
    }
}
