#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AA76C0[];

void func_08AA67A0(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AA76C0);
}

}
