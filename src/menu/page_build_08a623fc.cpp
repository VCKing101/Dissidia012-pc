#include "menu/agent6_types.h"

struct BuildPage {
    char pad_000[0xC8];
    char field_C8[0x1C];
    char field_E4[0xD14 - 0xE4];
    A6Part *field_D14[7];
    char pad_D30[0xDE4 - 0xD30];
    char field_DE4[4];
};

extern "C" char D_08A637C0[];
extern "C" char D_08A63800[];
extern "C" void func_088D3F00(char *dst, char *src, char *name);
extern "C" void func_088D44EC(char *dst, char *src, char *name);
extern "C" void func_088D4B74(char *list, int value);

extern "C" {
void func_08A623FC(BuildPage *self)
{
    int i;
    func_088D3F00(0, self->field_C8, D_08A637C0);
    func_088D44EC(self->field_C8, self->field_E4, D_08A63800);
    for (i = 0; i < 7; i++) {
        self->field_D14[i]->v1();
    }
    func_088D4B74(self->field_DE4, 0);
}
}
