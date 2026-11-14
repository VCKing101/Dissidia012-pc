#include "menu/auto_region3.h"

/* ScreenA3F44: per-frame updates by flag (1 without 2, 0x20, 0x40, 0x80). */
void func_08A9C140(ScreenA3F44 *self, int arg)
{
    unsigned int flags;

    flags = self->work->flags;
    if (((flags & 1) != 0) && ((flags & 2) == 0)) {
        func_08AB43FC(&self->field_894, arg);
        flags = self->work->flags;
    }
    if ((flags & 0x20) != 0) {
        func_08AA2AC4(self);
        flags = self->work->flags;
    }
    if ((flags & 0x40) != 0) {
        func_08AA2E20(self);
        flags = self->work->flags;
    }
    if ((flags & 0x80) != 0) {
        func_08AA32F8(self);
    }
}
