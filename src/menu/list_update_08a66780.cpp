#include "menu/agent12_types.h"

struct ListPage {
    char pad_000[0xC8];
    A12Part0C *field_C8[0x18];
};

extern "C" {
void func_08A66780(ListPage *self)
{
    for (int i = 0; i < 0x18; i++) {
        self->field_C8[i]->v5();
    }
}
}
