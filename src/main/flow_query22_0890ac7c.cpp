#include "main/agent21_types.h"

extern "C" {
int func_0890AC7C(DataFlow *flow, int arg)
{
    if (flow->client != 0) {
        return flow->client->query22(arg);
    }
    return 0;
}
}
