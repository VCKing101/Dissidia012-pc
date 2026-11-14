#include "menu/auto_region2.h"

/* MenuRoot2: makes a text from its messages (kind 0). */
void *func_08A412A8(void *root, int id, int arg1, int arg2)
{
    return func_0887B3EC(((MenuRoot2 *)root)->work->messages, 0, 0, id, arg1, arg2);
}
