#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AADFB8[];

void func_08AAB1B8(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AADFB8);
}

}
