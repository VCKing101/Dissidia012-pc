#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08AADFA8[];

void func_08AAB220(TableAt08Object *object, int flags)
{
    TableAt08Object::destroy(object, flags, D_08AADFA8);
}

}
