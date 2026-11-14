#include "menu/auto_region2.h"

/* Points out[0..8] at fields of a data record (three shorts from +0x15C, then six fields). */
void func_08A51598(void *self, char **out, char *data)
{
    char **slot;
    char *field;
    int i;

    i = 0;
    field = data + 0x15C;
    slot = out;
    do {
        *slot = field;
        i = i + 1;
        field = field + 2;
        slot = slot + 1;
    } while (i < 3);
    out[3] = data + 0x50;
    out[4] = data + 0x6A;
    out[5] = data + 0x14;
    out[6] = data + 0x2E;
    out[7] = data + 0x1A8;
    out[8] = data + 0xD;
}
