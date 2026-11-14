#include "main/agent21_types.h"

extern void func_08904038(int target, SaveEntry *entry, int arg0, int arg1);

void func_0890BDD8(SaveQueue *queue, int target)
{
    SaveRequest *req = &queue->pending;
    if (req->active == 1) {
        func_08904038(target, &queue->entries[req->index], req->arg0, req->arg1);
        req->active = 0;
    }
}
