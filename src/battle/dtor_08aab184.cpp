#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AADF88[];

void func_08AAB184(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AADF88);
}

}
