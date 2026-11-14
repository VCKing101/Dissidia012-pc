#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08ABC5E0[];

void func_08ABBFB4(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08ABC5E0);
}

}
