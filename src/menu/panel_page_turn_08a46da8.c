#include "menu/agent36_types.h"

typedef struct Work46DA8 {
    char pad_000[0x3E4];
    int flags;                /* +0x3E4 */
    char pad_3E8[0x8];
    int field_3F0;            /* +0x3F0 */
    char pad_3F4[0x4];
    int field_3F8;            /* +0x3F8 */
    char pad_3FC[0x14];
    void *table;              /* +0x410 */
} Work46DA8;

typedef struct Screen46DA8 {
    char pad_000[0x418];
    Work46DA8 *work;          /* +0x418 */
} Screen46DA8;

int func_08A484D0(void *table);
int func_08A48450(void *table, int key, int column);
int func_08A46F28(Screen46DA8 *self, int row);

void func_08A46DA8(Screen46DA8 *self, int forward, int base, int offset)
{
    int value[1];
    int page[1];
    int top[2];
    int count = func_08A484D0(self->work->table);
    int pages = 4;
    int rows = 3;
    int last = count - 8;

    if (self->work->flags & 0x200) {
        pages = 5;
    }
    value[0] = func_08A48450(self->work->table, base + offset, 3);
    page[0] = value[0] / rows;
    if (forward == 0) {
        page[0] = page[0] - 1;
        if (page[0] < 0) {
            page[0] = pages - 1;
        }
    } else {
        page[0] = page[0] + 1;
        if (page[0] >= pages) {
            page[0] = 0;
        }
    }
    top[0] = func_08A46F28(self, page[0] * rows);
    if (page[0] == 0) {
        self->work->field_3F8 = 0;
        self->work->field_3F0 = 0;
    } else if (page[0] == 1 || page[0] == 2 || page[0] == rows) {
        self->work->field_3F8 = top[0] - 1;
        self->work->field_3F0 = 1;
    } else {
        self->work->field_3F8 = top[0] - 7;
        self->work->field_3F0 = 7;
    }
    if (self->work->field_3F8 >= last) {
        self->work->field_3F8 = last;
    }
}
