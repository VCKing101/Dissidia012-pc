#include "menu/auto_region3.h"

/* ScreenA3F44: picks the next state from func_08A9B0DC's result. */
void func_08A98D88(ScreenA3F44 *self)
{
    int result;

    result = func_08A9B0DC(self);
    if (result != 0) {
        if (result == 1) {
            func_08A9B950(self);
            self->work->state = 0x1A;
            self->work->next = 0x1C;
        }
        else if (result == 2) {
            self->work->state = 0x1A;
            self->work->next = 0x14;
            if ((self->work->flags & 0x40) != 0) {
                self->work->next = 0x10;
            }
            if (D_09B6EFB0 == 2) {
                self->work->next = 0x10;
            }
        }
    }
}
