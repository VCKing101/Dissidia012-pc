#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AA7740[];

void func_08AA6940(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AA7740);
}

}
