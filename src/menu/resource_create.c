#include "menu/resource_phases.h"

void func_089DDD18(MenuRuntimePrefix *self) {
    self->field_01DC->field_0018 = func_088D0700(D_089D7830, 0x13, 0x4A08, 0x4804);
    self->field_01DC->field_001C = func_088DAA40(self->field_01DC->field_0018, 0, 0);
    self->field_01DC->field_001C->field_000E = 1;
    self->field_01DC->field_0010 = func_089C5FB8();
    self->field_01DC->field_0008++;
}
