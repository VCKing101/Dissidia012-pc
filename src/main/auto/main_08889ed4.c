#include "main/auto_region2.h"

/* Sets callback index of D_089D6A10 and its flag. */
void func_08889ED4(int index, FlowCallback callback, unsigned char flag)
{
    D_089D6A10->callbacks[index] = callback;
    D_089D6A10->callback_flags[index] = flag;
}
