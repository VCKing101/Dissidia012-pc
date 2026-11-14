#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08ABC5B0[];

void func_08ABBF18(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08ABC5B0);
}

}
