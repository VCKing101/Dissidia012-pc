#include "battle/auto_region4.h"

/* PartBBDE8 constructor. */
PartBBDE8 *func_08ABACA8(PartBBDE8 *self)
{
    self->vtable = D_08ABBDE8;
    func_08AB97E8(&self->field_00);
    func_08ABBE00(&self->field_60);
    func_08ABA7E8(&self->field_B4);
    func_08ABDE3C(&self->field_DC);
    func_08ABD5D0(&self->field_15C);
    func_08AC1E78(&self->field_16C);
    func_08ABA008(&self->field_1A4);
    func_08AC0AD0(&self->field_1B8);
    func_08AB7570(&self->field_1C8);
    func_08AB73E0(&self->field_1D8);
    func_08ABAE18(self);
    return self;
}
