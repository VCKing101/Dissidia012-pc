#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AADFE8[];

void func_08AAB2BC(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AADFE8);
}

}
