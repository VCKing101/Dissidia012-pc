#include "menu/agent9_types.h"

struct SlotOwner {
    char pad_000[0xD14];
    A9Ret1 *field_D14[31];
    int field_D90;
};

extern "C" {

int func_08A62F88(SlotOwner *self);

int func_08A630D4(SlotOwner *self)
{
    int index = self->field_D90;
    int value = func_08A62F88(self);
    if (self->field_D14[index]->r10(value) != 0) {
        return index;
    }
    return -1;
}

}
