#include "menu/agent6_types.h"

struct A6Select2 {
    char pad_000[0xD14];
    A6PartArg *field_D14[4];
    char pad_D24[0x6C];
    int field_D90;
};

extern "C" {
int func_08A62F88(A6Select2 *self);
void func_08A6324C(A6Select2 *self);

void func_08A63024(A6Select2 *self) {
    int idx = self->field_D90;
    int v = func_08A62F88(self);
    self->field_D14[idx]->w8(v);
}

void func_08A6307C(A6Select2 *self) {
    int idx = self->field_D90;
    int v = func_08A62F88(self);
    self->field_D14[idx]->w9(v);
}
}
