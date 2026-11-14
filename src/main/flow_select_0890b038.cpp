#include "main/agent21_types.h"

extern "C" {
void func_0890B038(DataFlow *flow, int arg)
{
    FlowClient *client = flow->client;
    if (client != 0) {
        client->onSelect(arg);
    }
    FlowSink *sink = flow->sink;
    if (sink != 0) {
        sink->onSelect(arg);
    }
}
}
