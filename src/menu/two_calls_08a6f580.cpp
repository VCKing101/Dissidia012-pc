#include "menu/agent9_types.h"

struct CallOwner {
    char pad_00[4];
    A9Virt3 *field_04;
    char pad_08[0x1D4 - 8];
    int field_1D4;
};

extern "C" {

void func_08A6E32C(void *target, void *owner, int a, int b);

void func_08A6F580(CallOwner *self, int value)
{
    func_08A6E32C(&self->field_1D4, self, 9, value);
    self->field_04->c0(9, value, 4);
}

}
