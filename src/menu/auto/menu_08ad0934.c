#include "menu/auto_region3.h"

/* ScreenDB30: counts a text that starts loading (func_08AD11A0 is called when it is done). */
void func_08AD0934(ScreenDB30 *self, void *text)
{
    int started;

    if ((text != 0) && (started = func_0887BA80(text, func_08AD11A0, self), started != 0)) {
        self->work->field_DB08 = self->work->field_DB08 + 1;
    }
}
