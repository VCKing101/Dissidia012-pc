#include "main/agent21_types.h"

extern "C" {
void func_08909F6C(DataFlow *flow)
{
    if (flow->listener != 0) {
        flow->listener->onFinished();
    }
    flow->state = 2;
}
}
