#include "menu/agent9_types.h"

struct ArgsOwner {
    char pad_000[0x208];
    A9Virt4 *field_208;
};

extern "C" {

void func_08A6FEC8(ArgsOwner *self, int a, int b, int c)
{
    A9Virt4 *obj = self->field_208;
    if (obj != 0) {
        obj->d3((int)self, a, b, c);
    }
}

}
