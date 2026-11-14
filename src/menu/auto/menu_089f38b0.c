#include "menu/auto_region1.h"

/* Screen0B6A0: moves field_2736 by delta within 0..func_089F0064() and keeps
 * the start and distance of the move (field_3224, field_3228). */
void func_089F38B0(Screen0B6A0 *self, short delta)
{
    int value;
    int index;

    index = (unsigned int)self->work->field_2736 + (int)delta;
    if (index < 0) {
        index = 0;
    } else {
        value = func_089F0064(self);
        if (value < index) {
            index = func_089F0064(self);
        }
    }
    value = func_089F00C8(self, self->work->field_2736);
    self->work->field_3224 = value;
    self->work->field_2736 = (unsigned short)index;
    index = func_089F00C8(self, self->work->field_2736);
    self->work->field_3228 = index - value;
    self->work->field_322C = 10;
    self->work->field_322E = 0;
    func_089F3B70(self);
}
