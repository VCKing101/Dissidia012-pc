#include "menu/agent36_types.h"

typedef struct Work46768 {
    char pad_000[0x410];
    void *table;              /* +0x410 */
} Work46768;

typedef struct Screen46768 {
    char pad_000[0x418];
    Work46768 *work;          /* +0x418 */
} Screen46768;

int func_08A48450(void *table, int key, int column);
int func_08A484D8(void *table, int column, int row);
void func_08A46238(Screen46768 *self, int on, int row, int column, int lit, int side);

void func_08A46768(Screen46768 *self, int skip, int base, int row, int column, int base2, int row2, int column2)
{
    int lit[1];
    int side[1];
    int index[2];
    int flags;

    if (skip == 0) {
        index[0] = func_08A48450(self->work->table, base + row, column);
        if (index[0] != -1) {
            flags = func_08A484D8(self->work->table, 0, index[0]);
            lit[0] = 0;
            if (flags & 2) {
                lit[0] = 1;
            }
            side[0] = 1;
            if (flags & 4) {
                side[0] = 0;
            }
            func_08A46238(self, 1, row, column, lit[0], side[0]);
        }
    }
    index[0] = func_08A48450(self->work->table, base2 + row2, column2);
    if (index[0] != -1) {
        flags = func_08A484D8(self->work->table, 0, index[0]);
        lit[0] = 0;
        if (flags & 2) {
            lit[0] = 1;
        }
        side[0] = 1;
        if (flags & 4) {
            side[0] = 2;
        }
        func_08A46238(self, 1, row2, column2, lit[0], side[0]);
    }
}
