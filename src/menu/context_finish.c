#include "menu/execution_state.h"
#include "menu/execution_callbacks.h"
extern void *D_089D712C;
extern unsigned char D_089D5F20;
extern void func_088AD524(void *, void *);
extern void func_08862444(void *);
void func_089DDC20(MenuRuntimePrefix *self) {
    if (self->field_01DC->field_0049) {
        self->field_01DC->field_004A = 0;
        func_088AD524(D_089D712C,(void *)func_089DE9EC);
    } else self->field_01DC->field_004A = 1;
    if (!D_089D5F20) {
        func_08862444(self->unknown_01E0);
        self->field_01DC->field_0078 = 1;
    } else self->field_01DC->field_0078 = 0;
    self->field_01DC->field_0008++;
}
