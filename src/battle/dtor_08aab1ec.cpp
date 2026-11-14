#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AADF98[];

void func_08AAB1EC(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AADF98);
}

}
