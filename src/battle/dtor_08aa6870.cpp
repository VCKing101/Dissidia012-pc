#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AA7700[];

void func_08AA6870(TableAt10Object *object, int flags)
{
    TableAt10Object::destroy(object, flags, D_08AA7700);
}

}
