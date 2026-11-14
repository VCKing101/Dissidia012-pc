#include "menu/auto_region2.h"

extern const CellSetup D_08A5DC38[];

/* Panel5DB80: sets up its Panel60670 and sixteen cells from D_08A5DC38. */
void func_08A5CE80(Panel5DB80 *self, void *messages)
{
    const CellSetup *setup;
    Cell5C5C8 *cell;
    int i;

    self->messages = messages;
    func_08A5D460(self);
    func_08A5D504(self, self->messages);
    func_08A602A4(&self->field_50, self->handles[0], 1, self->messages, 8, -1);
    i = 0;
    cell = self->cells;
    setup = D_08A5DC38;
    do {
        func_08A5C64C(cell, 5, setup->field_00, self->field_34, self->field_38, self->handles[0],
                      setup->field_08, setup->field_0C, self->messages);
        i = i + 1;
        cell = cell + 1;
        setup = setup + 1;
    } while (i < 16);
}
