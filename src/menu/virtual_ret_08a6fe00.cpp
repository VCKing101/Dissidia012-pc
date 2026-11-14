#include "menu/agent9_types.h"

struct RetOwner {
    char pad_000[0x208];
    A9Ret0 *field_208;
};

extern "C" {

int func_08A6FE00(RetOwner *self)
{
    if (self->field_208 != 0) {
        return self->field_208->q4();
    }
    return 0;
}

}
