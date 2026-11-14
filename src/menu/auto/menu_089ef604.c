#include "menu/auto_region1.h"

/* Screen0B6A0: for an entry of kind 2, runs func_089F1FEC on it and returns
 * the address of its field_0C; otherwise 0. */
unsigned char *func_089EF604(Screen0B6A0 *self, Entry84 *entry)
{
    unsigned char *result;

    result = 0;
    if (entry->kind == 2) {
        func_089F1FEC(self, entry);
        result = (unsigned char *)&entry->field_0C;
    }
    return result;
}
