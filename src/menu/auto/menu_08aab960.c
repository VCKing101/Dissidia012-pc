#include "menu/auto_region3.h"

/* ScreenAB7CFC: stores the current player's func_08AB1CDC value and sets its flag 0x20. */
void func_08AAB960(ScreenAB7CFC *self)
{
    PlayerBlock *block;
    unsigned int value;

    block = func_08AABC54();
    value = func_08AB1CDC(block->field_00[block->index]);
    func_08AB51F4(self->work->field_7CCC, block->index, value);
    block->field_14[block->index] = value;
    block->flags[block->index] = block->flags[block->index] | 0x20;
}
