#include "menu/agent12_types.h"

class ClosePanel : public A12Part24 {
public:
    char pad_028[0xE8 - 0x28];
    int field_E8[6];
};

extern "C" void func_08A55044(int handle);

extern "C" {
void func_08A576B4(ClosePanel *self)
{
    for (int i = 0; i < 6; i++) {
        func_08A55044(self->field_E8[i]);
    }
    self->v9();
    self->v8();
}
}
