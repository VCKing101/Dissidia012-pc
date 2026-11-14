#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AFB558[];

void func_08AF3E2C(TableAt10Object *object, int flags)
{
    TableAt10Object::destroy(object, flags, D_08AFB558);
}

}
