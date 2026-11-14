#include "menu/agent9_types.h"

struct VirtualHolder {
    A9Virt3 *field_00;
};

extern "C" {

void func_08A6E32C(VirtualHolder *self, int a, int b, int c)
{
    A9Virt3 *obj = self->field_00;
    if (obj != 0) {
        obj->c4(a, b, c);
    }
}

}
