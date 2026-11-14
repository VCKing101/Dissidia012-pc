#include "menu/auto_region4.h"

void func_08B38EF8(ScreenB41CC0 *self)
{
    StateB41CC0 *state;

    state = self->field_C4;
    if (state->field_C0 != 0) {
        self->flags = self->flags | 0x10;
        state->flags = state->flags | 4;
        func_089231A8(self->field_C4->field_C0);
    }
}
