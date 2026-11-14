#include "menu/auto_region3.h"

/* PanelAF08A4: clears its three texts. */
void func_08AF08F0(PanelAF08A4 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        self->texts[i] = 0;
    }
}
