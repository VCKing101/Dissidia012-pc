#include "main/agent21_types.h"
extern "C" {
int func_0890AA1C(DataFlow *flow, int arg)
{
    if (flow->client != 0) {
        return flow->client->onProgress(flow->request, arg, flow->field_198, flow->field_19C);
    }
    return 0;
}
}
