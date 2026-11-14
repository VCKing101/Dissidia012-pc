#include "battle/agent27_types.h"

extern int func_08B1A6B0(LoadSet *set);
extern int func_08B1C938(StreamSet *set);
extern int func_08B1A758(LoadSet *set);
extern void func_08B1C7B4(StreamSet *set);

int func_08B1D1E8(SoundManager *mgr)
{
    int loads = func_08B1A6B0(mgr->loads);
    int streams = func_08B1C938(mgr->streams);

    mgr->field_08 = (streams < loads) ? loads : streams;
    if (mgr->state == 2) {
        if (func_08B1A758(mgr->loads) == 1) {
            mgr->state = 3;
        }
    } else {
        func_08B1C7B4(mgr->streams);
    }
    return 0;
}
