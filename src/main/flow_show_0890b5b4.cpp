#include "main/agent21_types.h"

extern "C" {
void func_0890B5B4(DataFlow *flow)
{
    if (flow->sink != 0) {
        flow->sink->onShow();
    }
    flow->state = 3;
}
}
