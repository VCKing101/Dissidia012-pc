#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AA76F0[];

void func_08AA683C(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AA76F0);
}

}
