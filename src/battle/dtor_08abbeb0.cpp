#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08ABC590[];

void func_08ABBEB0(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08ABC590);
}

}
