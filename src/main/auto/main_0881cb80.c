#include "main/auto_region1.h"

/* The node after prev in the static Obj9C7530's list (the first for 0). */
SysNode *func_0881CB80(SysNode *prev)
{
    Obj9C7530 *system;
    SysNode *node;

    system = func_0881BAA8();
    if (prev == 0) {
        node = system->first;
    } else {
        node = prev->next;
    }
    return node;
}
