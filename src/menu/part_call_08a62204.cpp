#include "menu/agent6_types.h"

struct A6Select {
    char pad_000[0xD14];
    A6PartArg *field_D14[4];
    char pad_D24[0x6C];
    int field_D90;
};

extern "C" {
int func_08A62F88(A6Select *self);

void func_08A62204(A6Select *self) {
    int idx = self->field_D90;
    int v = func_08A62F88(self);
    self->field_D14[idx]->w11(v);
}
}
