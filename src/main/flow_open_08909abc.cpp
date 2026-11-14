#include "main/agent21_types.h"

extern "C" {
extern int func_08903D6C(void *request);

bool func_08909ABC(DataFlow *flow)
{
    if (func_08903D6C(flow->request) == 1) {
        flow->state = 2;
        return true;
    }
    return false;
}
}
