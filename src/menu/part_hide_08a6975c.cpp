#include "menu/agent12_types.h"

class HidePanel : public A12Part24 {
public:
    int pad_028;
    int field_2C;
};

extern "C" {
void func_08A6975C(HidePanel *self)
{
    self->v9();
    self->v8();
    self->field_2C = 0;
}
}
