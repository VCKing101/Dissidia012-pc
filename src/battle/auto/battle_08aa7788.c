#include "battle/auto_region4.h"

/* PartAAD68 constructor. */
PartAAD68 *func_08AA7788(PartAAD68 *self)
{
    self->vtable = D_08AAAD68;
    func_08AA6618(&self->field_08);
    func_08AA6618(&self->field_DC);
    func_08AA7840(self);
    return self;
}
