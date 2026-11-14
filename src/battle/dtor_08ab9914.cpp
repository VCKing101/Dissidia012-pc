#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AB9F58[];

void func_08AB9914(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AB9F58);
}

}
