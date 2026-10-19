#include "menu/resource_phases.h"

void func_089DE008(MenuRuntimePrefix *self) {
    int selected = !D_089D5F1F ? 0x4807 : 0x4806;
    self->field_01DC->field_0020 = func_088D0700(D_089D7830, 0x13, 0x4A06, selected);
    self->field_01DC->field_0024 = func_088DAA40(self->field_01DC->field_0020, 0, 0);
    self->field_01DC->field_0024->field_000E = 1;
    self->field_01DC->field_0010 = func_089C5FB8();
    self->field_01DC->field_0008++;
}
