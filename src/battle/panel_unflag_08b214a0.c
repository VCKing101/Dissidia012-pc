#include "battle/agent27_types.h"

int func_08B214A0(Panel *panel)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
            if (panel->links[i] != 0) {
                panel->links[i]->flags &= ~0x20;
            }
        }
    }
    return 0;
}
