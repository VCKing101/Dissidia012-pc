#include "main/agent21_types.h"

extern "C" {
void func_0890B4BC(DataFlow *flow, int code)
{
    flow->state = 7;
    if (flow->client != 0) {
        flow->client->onError(code);
    }
    FlowSink *sink = flow->sink;
    if (sink != 0) {
        sink->onError(code);
    }
}
}
