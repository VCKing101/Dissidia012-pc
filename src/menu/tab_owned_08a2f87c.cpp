#include "menu/agent36_types.h"

/* Tabbed pages: on list 1, whether the save data holds the current
 * record's item (the first or second one, by +0x6D7). */
extern "C" bool func_08A2F87C(TabScreen36 *self, ListPage36 *page)
{
    if (page->list == 1) {
        TabRecord36 *record = func_08A3C164(self->work->ids[1][self->work->cursor.index]);
        void *items = D_089D712C->items;
        if (self->work->field_6D7 == 0) {
            if (record->item_b != 0xFFFF && func_088B912C(items, record->item_b) != 0) {
                return true;
            }
        } else {
            if (record->item_a != 0xFFFF && func_088B912C(items, record->item_a) != 0) {
                return true;
            }
        }
    }
    return false;
}
