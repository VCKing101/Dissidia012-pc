#include "menu/auto_region3.h"

/* ObjAB35B8: frees its text. */
void func_08AB3BA0(ObjAB35B8 *self)
{
    int i;

    for (i = 0; i < 1; i++) {
        if (self->work->texts[i] != 0) {
            func_0887BA5C(self->work->texts[i]);
            self->work->texts[i] = 0;
        }
    }
}
