#include "menu/resource_phases.h"

void func_089DDE58(MenuRuntimePrefix *self) {
    self->field_01DC->field_0018 = func_088D0700(D_089D7830, 0x13, 0x4A04, 0x4802);
    self->field_01DC->field_001C = func_088DAA40(self->field_01DC->field_0018, 0, 0);
    self->field_01DC->field_001C->field_000E = 1;
    self->field_01DC->field_0010 = func_089C5FB8();
    self->field_01DC->field_0008++;
    if (self->field_01DC->field_0068) {
        func_0889F6A0(&self->subobjects_014C[2].prefix, 3,
                      self->field_01DC->field_0068, self->field_01DC->field_006C);
        func_0889F750(&self->subobjects_014C[2].prefix, 2, 0, 0, 1.0f);
    }
}
