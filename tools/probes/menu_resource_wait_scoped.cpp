extern "C" {
#include "menu/resource_inline.h"
}

extern "C" int func_0889F980(MainResourceHandlePrefix *);
extern "C" int func_0889F704(MainResourceHandlePrefix *);

extern "C" void func_089DDF24(MenuRuntimePrefix *self) {
    if (self->field_01DC->field_001C->field_000C == 2) {
        do {
        if (MenuResourceFlags::has_0020(self->field_01DC->field_001C)) {
            if (self->field_01DC->field_0068) {
                MainResourceHandlePrefix *handle = &self->subobjects_014C[2].prefix;
                if (func_0889F980(handle) != 1) break;
                func_0889F704(handle);
            }
            if (self->field_01DC->field_0018) {
                func_088DAB44(self->field_01DC->field_0018);
                self->field_01DC->field_0018 = 0;
            }
            self->field_01DC->field_0008++;
        }
        } while (0);
    } else {
        unsigned int value = func_089C5FB8();
        if (value - self->field_01DC->field_0010 >= 2000000u) {
            func_088D9F5C(self->field_01DC->field_001C, 2, 1);
        }
    }
}
