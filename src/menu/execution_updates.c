#include "menu/execution_state.h"

extern unsigned char D_09B6C2C0;
extern unsigned char D_089D5F1F;
extern void *D_089D712C;
extern void func_088AC82C(void *context, unsigned char flag);
extern int func_088AC890(void *context);

/* The repeated flag test and stores preserve possible aliasing/reload behavior;
 * original option names and the called service's purpose are unknown. */
void func_089DD674(MenuRuntimePrefix *self) {
    self->field_01DC->field_0049 = 0;
    if (D_09B6C2C0) self->field_01DC->field_0049 = 1;
    if (D_09B6C2C0) self->field_01DC->field_0049 = 1;
    if (D_089D5F1F) self->field_01DC->field_0049 = 0;
    func_088AC82C(D_089D712C, self->field_01DC->field_0049 & 0xFF);
    self->field_01DC->field_0008++;
}

void func_089DD70C(MenuRuntimePrefix *self) {
    int result = func_088AC890(D_089D712C);
    if (result) {
        self->field_01DC->field_004C = result == 1;
        self->field_01DC->field_0008++;
    }
}
