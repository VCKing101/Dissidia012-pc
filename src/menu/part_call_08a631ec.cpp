#include "menu/agent6_types.h"

struct A6Select3 {
    char pad_000[0xD14];
    A6PartArg *field_D14[4];
    char pad_D24[0x6C];
    int field_D90;
};

extern "C" {
int func_08A62F88(A6Select3 *self);
void func_08A6324C(A6Select3 *self);

void func_08A631EC(A6Select3 *self) {
    func_08A6324C(self);
    int idx = self->field_D90;
    int v = func_08A62F88(self);
    self->field_D14[idx]->w4(v);
}
}
