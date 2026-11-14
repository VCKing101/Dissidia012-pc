#include "main/agent21_types.h"

extern "C" {
extern void func_089035DC(void *request);
extern char D_089CDD60[];
extern char D_089CDD78[];

struct FlowInit {
    static DataFlow *construct(DataFlow *flow)
    {
        flow->field_000 = 0;
        func_089035DC(flow->request);
        flow->state = 0;
        flow->field_194 = -1;
        flow->onDone.handlers = D_089CDD60;
        flow->onDone.owner = flow;
        flow->onError.handlers = D_089CDD78;
        flow->onError.owner = flow;
        flow->sink = 0;
        flow->field_1B8 = 0;
        flow->client = 0;
        flow->field_198 = 0;
        flow->field_19C = 0;
        return flow;
    }
};

DataFlow *func_0890A470(DataFlow *flow)
{
    return FlowInit::construct(flow);
}
}
