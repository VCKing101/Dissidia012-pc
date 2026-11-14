#include "main/agent21_types.h"

extern "C" {
void func_08909C4C(DataFlow *flow)
{
    if (flow->listener != 0) {
        flow->listener->onBegin();
    }
    flow->state = 2;
}
}
