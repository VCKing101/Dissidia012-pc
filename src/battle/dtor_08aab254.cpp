#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AADFC8[];

void func_08AAB254(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AADFC8);
}

}
