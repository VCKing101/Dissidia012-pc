#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AAE158[];

void func_08AAB838(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AAE158);
}

}
