#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AB9F88[];

void func_08AB99B0(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AB9F88);
}

}
