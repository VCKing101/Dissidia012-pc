#include "main/agent21_types.h"

extern "C" {
void func_0890AD18(DataFlow *flow)
{
    FlowClient *client = flow->client;
    if (client != 0) {
        client->onOpen(flow->request);
    }
}
}
