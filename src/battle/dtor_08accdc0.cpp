#include "battle/agent14_types.h"

extern "C" {

extern const unsigned char D_08ACD0C8[];

void func_08ACCDC0(TableAt10Object *object, int flags)
{
    TableAt10Object::destroy(object, flags, D_08ACD0C8);
}

}
