#include "main/agent21_types.h"

extern "C" {
void func_0890ADD0(DataFlow *flow)
{
    FlowClient *client = flow->client;
    if (client != 0 && client->isBusy(flow->request) == 0) {
        FlowSink *sink = flow->sink;
        if (sink != 0) {
            sink->setActive(0);
        }
        flow->state = 1;
    }
}
}
