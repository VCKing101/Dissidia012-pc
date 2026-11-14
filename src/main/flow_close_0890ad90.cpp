#include "main/agent21_types.h"

extern "C" {
void func_0890AD90(DataFlow *flow)
{
    FlowClient *client = flow->client;
    if (client != 0) {
        client->onClose(flow->request);
    }
}
}
