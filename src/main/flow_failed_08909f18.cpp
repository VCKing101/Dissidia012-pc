#include "main/agent21_types.h"

extern "C" {
void func_08909F18(DataFlow *flow)
{
    if (flow->listener != 0) {
        flow->listener->onFailed();
    }
    flow->state = 2;
}
}
