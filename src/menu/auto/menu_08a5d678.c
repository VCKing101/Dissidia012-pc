#include "menu/auto_region2.h"

/* Panel5DB80: highlights the selected cell (handle 2 when beyond them). */
void func_08A5D678(Panel5DB80 *self)
{
    Cell5C5C8 *cell;
    int i;
    int index;

    i = 0;
    cell = self->cells;
    do {
        func_08A5C708(cell);
        i = i + 1;
        cell = cell + 1;
    } while (i < 16);
    func_088D9CFC(self->handles[2]);
    index = func_08A5D654(self);
    if (index < 16) {
        func_08A5C6E8(&self->cells[index]);
    } else {
        func_088D9CD8(self->handles[2]);
    }
}
