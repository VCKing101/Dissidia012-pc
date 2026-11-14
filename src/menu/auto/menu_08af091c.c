#include "menu/auto_region3.h"

/* PanelAF08A4: frees its three texts. */
void func_08AF091C(PanelAF08A4 *self)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (self->texts[i] != 0) {
            func_0887BA5C(self->texts[i]);
            self->texts[i] = 0;
        }
    }
}
