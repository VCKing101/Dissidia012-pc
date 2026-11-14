#include "menu/agent12_types.h"

class RefreshPart : public A12Part0C {
public:
    char pad_010[0x68 - 0x10];
    int field_68;
    int field_6C;
};

extern "C" void func_088D688C(int *anim, int value);

extern "C" {
void func_08A65044(RefreshPart *self)
{
    if (self->field_02 == 0) {
        return;
    }
    if (self->field_00 == 0) {
        return;
    }
    int frame = self->field_6C;
    func_088D688C(&self->field_68, 0);
    if (frame == self->field_6C) {
        return;
    }
    self->v19();
}
}
