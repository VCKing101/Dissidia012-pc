#include "battle/agent15_types.h"

extern "C" {

extern const unsigned char D_08AED2A0[];

void func_08AECDD4(TableAt04NodeObject *object, int flags)
{
    TableAt04NodeObject::destroy(object, flags, D_08AED2A0);
}

}
