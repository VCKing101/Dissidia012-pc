#include "menu/agent12_types.h"

class OpenPanel : public A12Part24 {
public:
    char pad_028[0x34 - 0x28];
    int field_34;
    char pad_038[0x44 - 0x38];
    char field_44[0x10];
    int field_54;
};

extern "C" char D_08A5B6F8[];
extern "C" int func_0887B3EC(int layout, int kind, int a, int b, int c, int d);
extern "C" void func_088D428C(char *dst, int *src, char *name, int flag);

extern "C" {
void func_08A5A6DC(OpenPanel *self)
{
    self->v9();
    self->field_54 = func_0887B3EC(self->field_34, 8, 0, 0x2F, 0, 0);
    func_088D428C(self->field_44, &self->field_54, D_08A5B6F8, 1);
}
}
