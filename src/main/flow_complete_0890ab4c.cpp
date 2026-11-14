#include "main/agent21_types.h"

extern "C" {
int func_0890AB4C(DataFlow *flow, int a, int b)
{
    if (flow->state == 7) {
        if (flow->client != 0) {
            flow->client->onComplete(flow->request, a, b);
        }
        flow->state = 5;
        return 1;
    }
    return 0;
}
}
