extern "C" {
#include "menu/resource_inline.h"
extern int func_088D4C5C(void);
extern void func_08873D34(int, const void *, int, int, int, float);
extern void func_08873DB0(const float *, int);
extern unsigned char D_089DE9D4[];
}
extern "C" void func_089DE0A4(MenuRuntimePrefix *self) {
    int status = self->field_01DC->field_0024->field_000C;
    if (!self->field_01DC->field_0020) {
        if (self->field_01DC->field_0048)
            self->field_01DC->field_0008 = !D_089D5F1F ? 15 : 17;
    } else if (status == 2) {
        if (MenuResourceFlags::has_0020(self->field_01DC->field_0024)) {
            if (self->field_01DC->field_0020) {
                func_088DAB44(self->field_01DC->field_0020);
                self->field_01DC->field_0020 = 0;
            }
            self->field_01DC->field_0048 = 0;
            float value = (float)func_088D4C5C();
            func_08873D34(3,D_089DE9D4,0,1,2,value);
            float values[4] = {255.0f,255.0f,255.0f,255.0f};
            func_08873DB0(values,2);
        }
    } else {
        unsigned int counter = func_089C5FB8();
        if (counter-self->field_01DC->field_0010 >= 4000000u)
            func_088D9F5C(self->field_01DC->field_0024,2,1);
    }
}
