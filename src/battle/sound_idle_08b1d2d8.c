#include "battle/agent27_types.h"

extern unsigned char func_08B1D274(SoundManager *mgr);

int func_08B1D2D8(SoundManager *mgr)
{
    if (func_08B1D274(mgr) == 0) {
        return 0;
    }
    return mgr->state == 0;
}
