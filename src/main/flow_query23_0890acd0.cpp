#include "main/agent21_types.h"

extern "C" {
int func_0890ACD0(DataFlow *flow)
{
    if (flow->client != 0) {
        return flow->client->query23();
    }
    return 0;
}
}
