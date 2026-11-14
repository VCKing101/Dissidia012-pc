#include "menu/agent12_types.h"

struct ListPage {
    char pad_000[0xC8];
    A12Part0C *field_C8[0x18];
    char pad_128[0x1F9C - 0x128];
    unsigned char field_1F9C;
};

extern "C" {
void func_08A66AEC(ListPage *self)
{
    for (int i = 0; i < 0x18; i++) {
        self->field_C8[i]->v8();
    }
    self->field_1F9C = 1;
}
}
