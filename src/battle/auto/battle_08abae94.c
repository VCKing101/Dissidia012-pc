#include "battle/auto_region4.h"

/* PartBBDE8 release: releases the members from field_DC on, then resets. */
void func_08ABAE94(PartBBDE8 *self)
{
    func_08AB7498(&self->field_1D8);
    func_08AB7610(&self->field_1C8);
    func_08ABA0B4(&self->field_1A4);
    func_08AC0B70(&self->field_1B8);
    func_08AC1F48(&self->field_16C);
    func_08ABD670(&self->field_15C);
    func_08ABDF94(&self->field_DC);
    func_08ABAE18(self);
}
