#include "menu/agent12_types.h"

class HidePanel : public A12Part24 {
public:
    int field_28;
};

extern "C" {
void func_08A60408(HidePanel *self)
{
    self->v9();
    self->v8();
    self->field_28 = 0;
}
}
