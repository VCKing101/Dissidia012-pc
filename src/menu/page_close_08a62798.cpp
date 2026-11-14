#include "menu/agent12_types.h"

struct CloseRow {
    char data[0x240];
};

class ClosePage : public A12Part24 {
public:
    int field_28;
    char pad_02C[0xE4 - 0x2C];
    char field_E4[0x60];
    char field_144[0x90];
    CloseRow field_1D4[5];
};

extern "C" void func_08A54890(char *part);
extern "C" void func_08A52548(char *part);
extern "C" void func_08A5352C(CloseRow *row);

extern "C" {
void func_08A62798(ClosePage *self)
{
    func_08A54890(self->field_E4);
    func_08A52548(self->field_144);
    for (int i = 0; i < 5; i++) {
        func_08A5352C(&self->field_1D4[i]);
    }
    self->v9();
    self->v8();
    self->field_28 = 0;
}
}
