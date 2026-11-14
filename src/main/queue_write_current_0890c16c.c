#include "main/agent21_types.h"

extern void func_08904038(int target, SaveEntry *entry, int arg0, int arg1);

void func_0890C16C(SaveQueue *queue, int target, int arg0, int arg1)
{
    func_08904038(target, &queue->entries[queue->current], arg0, arg1);
}
