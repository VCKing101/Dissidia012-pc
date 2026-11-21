#include "menu/agent36_types.h"

/* Picker: steps to the next or previous entry; past either end it switches
 * mode (func_08A31F34), or wraps around when the mode stays the same. */

void func_08A31C70(Picker36 *self)
{
    int wrapped;
    int mode;

    if (self->work->list != 0) {
        wrapped = 0;
        if (self->work->forward != 0) {
            self->work->index++;
            if (self->work->count - 1 < self->work->index) {
                wrapped = 1;
            }
        } else {
            self->work->index--;
            if (self->work->index < 0) {
                wrapped = 1;
            }
        }
        if (!wrapped) {
            func_08A34AB4(self, &self->work->entries[self->work->index]);
            self->work->state = 8;
            self->work->flags |= 0x40;
        } else {
            mode = func_08A31F34(self);
            if (self->work->mode == mode) {
                self->work->index = self->work->forward ? 0 : self->work->count - 1;
                func_08A34AB4(self, &self->work->entries[self->work->index]);
                self->work->state = 8;
                self->work->flags |= 0x40;
            } else {
                self->work->mode = mode;
                func_08A31688(self);
                self->work->flags |= 0x10;
            }
        }
    }
}
