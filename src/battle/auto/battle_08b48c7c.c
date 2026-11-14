#include "battle/auto_region5.h"

/* Adds entry `id`, and every entry of its group, to the list from
 * func_0891705C when their flag 1 is set. */
void func_08B48C7C(void *self, int id)
{
    short group;
    void *list;
    Entry48C7C *entry;

    list = func_0891705C(-1);
    entry = func_08912FA4(id);
    if ((entry->flags & 1) != 0) {
        func_08912B04(list, entry->field_02);
    }
    group = entry->field_04;
    entry = 0;
    while ((entry = func_08912F7C(group, entry)) != 0) {
        if ((entry->flags & 1) != 0) {
            func_08912B04(list, entry->field_02);
        }
    }
}
