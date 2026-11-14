#include "main/agent21_types.h"

extern "C" {
void func_0890B310(DataFlow *flow, int arg)
{
    FlowClient *client = flow->client;
    if (client != 0) {
        client->onRetry(arg);
    }
    FlowSink *sink = flow->sink;
    if (sink != 0) {
        sink->onRetry(arg);
    }
}
}
