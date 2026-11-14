#include "battle/auto_region1.h"

/* UnitF0C68 constructor. */
UnitF0C68 *func_089EFC90(UnitF0C68 *self)
{
    func_08A157D0(self);
    self->vtable = D_089F0C68;
    func_089EF290(&self->field_1C);
    func_089F8208(&self->field_44);
    func_08A0E398(self->field_7C);
    func_08A08178(self->field_114);
    func_089EFD80(self);
    return self;
}
