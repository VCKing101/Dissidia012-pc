#include "menu/agent12_types.h"

struct OpenList {
    char pad_000[0x38];
    char field_38[0x6C];
    char field_A4[0x24];
    A12Part0C *field_C8[0x18];
};

extern "C" char D_08A6825C[];
extern "C" void func_088D3F00(char *dst, char *src, char *name);
extern "C" void func_088D4B74(char *list, int value);

extern "C" {
void func_08A66CE8(OpenList *self)
{
    int i;
    func_088D3F00(0, self->field_38, D_08A6825C);
    for (i = 0; i < 0x18; i++) {
        self->field_C8[i]->v9();
    }
    func_088D4B74(self->field_A4, 0);
}
}
