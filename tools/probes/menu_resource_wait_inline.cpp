/* Nonmatching exploration: not part of the C manifest and earns no source credit. */
extern "C" {
#include "menu/resource_phases.h"
}

struct ReadyView : MenuResourceView {
 bool ready() { return (field_0020 & 0x20) != 0; }
};
extern "C" void func_089DDDA0(MenuRuntimePrefix *self) {
    if (self->field_01DC->field_001C->field_000C == 2) {
        if (((ReadyView *)self->field_01DC->field_001C)->ready()) {
            if (self->field_01DC->field_0018) {
                func_088DAB44(self->field_01DC->field_0018);
                self->field_01DC->field_0018 = 0;
            }
            self->field_01DC->field_0008++;
        }
    } else {
        unsigned int value = func_089C5FB8();
        if (value - self->field_01DC->field_0010 >= 2000000u)
            func_088D9F5C(self->field_01DC->field_001C, 2, 1);
    }
}
