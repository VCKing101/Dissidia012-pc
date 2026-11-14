#include "battle/agent27_types.h"

typedef struct PanelRef {
    int field_00;
    PanelLink *link;    /* 0x04 */
} PanelRef;

int func_08B214FC(PanelRef *ref)
{
    if (ref->link != 0) {
        ref->link->flags &= ~0x20;
    }
    return 0;
}
