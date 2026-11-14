#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08ABC5C0[];

void func_08ABBF4C(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08ABC5C0);
}

}
