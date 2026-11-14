#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AADFF8[];

void func_08AAB2F0(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AADFF8);
}

}
