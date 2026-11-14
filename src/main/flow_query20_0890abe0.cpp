#include "main/agent21_types.h"

extern "C" {
int func_0890ABE0(DataFlow *flow, int arg)
{
    if (flow->client != 0) {
        return flow->client->query20(arg);
    }
    return 0;
}
}
