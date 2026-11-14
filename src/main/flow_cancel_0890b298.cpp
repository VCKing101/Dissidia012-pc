#include "main/agent21_types.h"

extern "C" {
void func_0890B298(DataFlow *flow, int arg)
{
    FlowClient *client = flow->client;
    if (client != 0) {
        client->onCancel(arg);
    }
    FlowSink *sink = flow->sink;
    if (sink != 0) {
        sink->onCancel(arg);
    }
}
}
