#include "battle/agent27_types.h"

extern int func_0887AFB0(void *object);

unsigned char func_08B1D274(SoundManager *mgr)
{
    int result = 0;

    if (mgr->active == 1 && func_0887AFB0(mgr->field_14) == 1 && mgr->ready == 1) {
        result = 1;
    }
    return result;
}
