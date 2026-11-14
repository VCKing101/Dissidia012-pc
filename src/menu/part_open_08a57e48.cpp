#include "menu/agent12_types.h"

class OpenPanel : public A12Part24 {
public:
    char pad_028[0x30 - 0x28];
    int field_30;
    char pad_034[0xD0 - 0x34];
    char field_D0[0x14];
    int field_E4;
};

extern "C" char D_08A5862C[];
extern "C" int func_0887B3EC(int layout, int kind, int a, int b, int c, int d);
extern "C" void func_088D428C(char *dst, int *src, char *name, int flag);

extern "C" {
void func_08A57E48(OpenPanel *self)
{
    self->v9();
    self->field_E4 = func_0887B3EC(self->field_30, 8, 0, 0x12, 0, 0);
    func_088D428C(self->field_D0, &self->field_E4, D_08A5862C, 1);
}
}
