#include "main/agent21_types.h"

extern "C" {
void func_08909CA0(DataFlow *flow)
{
    if (flow->listener != 0) {
        flow->listener->onReset();
    }
    flow->state = 0;
}
}
