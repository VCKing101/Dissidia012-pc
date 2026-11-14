#include "main/agent21_types.h"

extern "C" {
int func_0890AC34(DataFlow *flow)
{
    if (flow->client != 0) {
        return flow->client->query21();
    }
    return 0;
}
}
