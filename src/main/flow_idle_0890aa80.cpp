#include "main/agent21_types.h"

extern "C" {
int func_0890AA80(DataFlow *flow)
{
    if (flow->state == 7) {
        return 0;
    }
    if (flow->client != 0) {
        return flow->client->onIdle(flow->request);
    }
    return 0;
}
}
