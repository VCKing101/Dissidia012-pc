#include "main/agent21_types.h"

extern "C" {
extern int func_08903E20(void *request, int a, int b, int c);

bool func_08909B04(DataFlow *flow)
{
    if (func_08903E20(flow->request, 0, 0, 0) == 0) {
        flow->state = 2;
        return false;
    }
    flow->field_14C58 = 0;
    return true;
}
}
