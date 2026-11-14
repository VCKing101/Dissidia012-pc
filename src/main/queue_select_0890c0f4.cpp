#include "main/agent21_types.h"

extern "C" {
extern int func_08903CAC(int target, int index, int arg0, int arg1);

bool func_0890C0F4(SaveQueue *queue, int target, int index, int arg0, int arg1)
{
    if (func_08903CAC(target, index, arg0, arg1) == 1) {
        queue->current = index;
        return true;
    }
    return false;
}
}
