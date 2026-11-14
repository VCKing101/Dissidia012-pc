#include "main/agent21_types.h"

extern "C" {
void func_0890B0B0(DataFlow *flow, int arg)
{
    if (flow->sink != 0) {
        flow->sink->onNotify(arg);
    }
}
}
