#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AB9F78[];

void func_08AB997C(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AB9F78);
}

}
