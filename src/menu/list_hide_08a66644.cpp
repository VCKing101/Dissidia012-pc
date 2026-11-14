#include "menu/agent12_types.h"

struct ListPage {
    char pad_000[0x28];
    int field_28;
    char pad_02C[0xC8 - 0x2C];
    A12Part0C *field_C8[0x18];
};

extern "C" {
void func_08A66644(ListPage *self)
{
    for (int i = 0; i < 0x18; i++) {
        self->field_C8[i]->v8();
    }
    self->field_28 = 2;
}
}
