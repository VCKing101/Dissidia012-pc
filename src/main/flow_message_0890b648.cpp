#include "main/agent21_types.h"

extern "C" {
void func_0890B648(DataFlow *flow, int arg)
{
    if (flow->sink != 0) {
        flow->sink->onMessage(arg);
    }
}
}
