#include "main/agent21_types.h"

extern "C" {
void func_08909ED8(DataFlow *flow)
{
    if (flow->listener != 0) {
        flow->listener->onUpdate();
    }
}
}
