#include "main/agent21_types.h"

extern "C" {
extern int func_08903E20(int target, int index, int arg0, int arg1);

bool func_0890BE34(SaveQueue *queue, int target)
{
    SaveRequest *req = &queue->opening;
    if (req->active == 0) {
        if (func_08903E20(target, req->index, req->arg0, req->arg1) != 1) {
            return false;
        }
        req->active = 1;
    }
    return true;
}
}
