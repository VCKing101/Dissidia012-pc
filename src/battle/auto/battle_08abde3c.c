#include "battle/auto_region4.h"

/* PartC03B8 constructor: clears the four list heads and builds the three
 * at field_4C. */
PartC03B8 *func_08ABDE3C(PartC03B8 *self)
{
    self->vtable = D_08AC03B8;
    self->field_1C.field_04 = 0;
    self->field_1C.first = 0;
    self->field_1C.field_08 = 0;
    self->field_28.field_04 = 0;
    self->field_28.first = 0;
    self->field_28.field_08 = 0;
    self->field_34.field_04 = 0;
    self->field_34.first = 0;
    self->field_34.field_08 = 0;
    self->field_40.field_04 = 0;
    self->field_40.first = 0;
    self->field_40.field_08 = 0;
    func_08993560(self->field_4C, 3, sizeof(ListHead), func_08ABDE28);
    func_08ABDF14(self);
    return self;
}
