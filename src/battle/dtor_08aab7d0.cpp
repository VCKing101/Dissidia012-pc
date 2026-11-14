#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AAE0D8[];

void func_08AAB7D0(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AAE0D8);
}

}
