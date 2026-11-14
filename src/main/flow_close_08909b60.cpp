#include "main/agent21_types.h"

extern "C" {
extern int func_08903EC4(void *request, int mode);

bool func_08909B60(DataFlow *flow)
{
    if (func_08903EC4(flow->request, 0) == 1) {
        flow->state = 2;
        return true;
    }
    return false;
}
}
