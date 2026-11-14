#include "main/agent21_types.h"

extern "C" {
void func_0890A1BC(DataFlow *flow)
{
    if (flow->listener != 0) {
        flow->listener->onClosed();
    }
}
}
