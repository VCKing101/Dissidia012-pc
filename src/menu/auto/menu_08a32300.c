#include "menu/auto_region1.h"

/* Screen35A00: the first value of entry id in its table. */
int func_08A32300(Screen35A00 *self, int id)
{
    int table;
    short *value;

    table = func_08A3200C(self);
    value = func_088470DC(table, id, 0);
    return *value;
}
