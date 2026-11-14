#include "main/agent21_types.h"

extern "C" {
extern int func_08935490(void *files, int id, int count);

struct FlowFiles {
    static bool missing(DataFlow *flow, int id) { return func_08935490(flow->files, id, 6) == 0; }
};

void func_0890A1FC(DataFlow *flow, int id)
{
    if (FlowFiles::missing(flow, id)) {
        if (flow->listener != 0) {
            flow->listener->onFailed();
        }
        flow->state = 2;
    }
}
}
