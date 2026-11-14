#include "menu/agent12_types.h"

class HidePanel : public A12Part24 {
public:
    int field_28;
    char pad_02C[0x218 - 0x2C];
    char field_218[4];
};

extern "C" void func_08A576B4(char *panel);

extern "C" {
void func_08A6B6CC(HidePanel *self)
{
    self->v9();
    self->v8();
    func_08A576B4(self->field_218);
    self->field_28 = 0;
}
}
