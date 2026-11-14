#include "main/agent21_types.h"

extern "C" {
void func_0890B600(DataFlow *flow)
{
    if (flow->sink != 0) {
        flow->sink->onHide();
    }
    flow->state = 0;
}
}
