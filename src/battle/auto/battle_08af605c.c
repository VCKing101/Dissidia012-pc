#include "battle/auto_region5.h"

/* PartF6008 release: releases the members, the nine resources and the five
 * models, and clears field_F34. */
void func_08AF605C(PartF6008 *self)
{
    int i;

    func_08AF42C0(self->field_50);
    func_08AF49BC(self->field_B0);
    func_08AF522C(self->field_144);
    func_08B4CC24(self->field_2E0);
    func_08B4B430(self->field_428);
    func_088CC51C(self->field_678);
    func_08B4C2B8(&self->field_688);
    func_08B4C2B8(&self->field_6A0);
    if (self->field_F54 != 0 && self->field_F54 != 7) {
        func_08AF5A04(self->field_6AC);
    }
    for (i = 0; i < 9; i++) {
        if (self->handles[i] != 0) {
            func_088DA060(self->handles[i]);
            self->handles[i] = 0;
        }
    }
    for (i = 0; i < 5; i++) {
        if (self->models[i] != 0) {
            func_0887BA5C(self->models[i]);
            self->models[i] = 0;
        }
    }
    self->field_F34 = 0;
}
