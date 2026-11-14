#include "battle/agent27_types.h"

extern unsigned char func_08B1D274(SoundManager *mgr);
extern int func_08B1A7C4(LoadSet *set);
extern int func_08B1CA0C(StreamSet *set);

int func_08B1D314(SoundManager *mgr)
{
    if (func_08B1D274(mgr) == 0) {
        return -1;
    }
    if (func_08B1A7C4(mgr->loads) != 0) {
        return -2;
    }
    if (func_08B1CA0C(mgr->streams) != 0) {
        return -3;
    }
    mgr->state = 1;
    return 0;
}
