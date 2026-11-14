#include "menu/auto_region2.h"

/* Panel6C638: opens on value, restoring a saved cursor index, state 1. */
void func_08A6B638(Panel6C638 *self, int value)
{
    self->field_30 = value;
    self->field_2C = 0;
    func_08A6BABC(self);
    func_08A6BB60(self, value);
    func_08A6BDBC(self);
    if (self->field_214 != -1) {
        self->cursor.index = self->field_214;
        self->field_214 = -1;
    }
    func_08A6BDEC(self);
    func_08A6BED0(self);
    func_08A6C03C(self);
    func_08A6C3EC(self);
    func_08A6C144(self);
    self->state = 1;
}
