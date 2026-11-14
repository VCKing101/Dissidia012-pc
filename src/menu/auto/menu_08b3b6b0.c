#include "menu/auto_region4.h"

/* ScreenB41CC0: releases the messages of its parts. */
void func_08B3B6B0(ScreenB41CC0 *self)
{
    if (self->field_C8->messages != 0) {
        func_0887B1F4(self->field_C8->messages, 3);
        self->field_C8->messages = 0;
    }
}
