#include "menu/agent29_types.h"

/* ScreenE75C4: opens the help window (script D_089E6E14) over layout
   entry 0xD with the screen's messages. */
extern unsigned char D_089D5F20;
extern const short D_089E6E14[];

void func_089E2C38(ScreenE75C4 *self)
{
    int mode = 1;
    char none = 0;

    if (D_089D5F20 != 0) {
        mode = 1;
    }
    func_088FD928(D_089D7F90, self->heap, 0, 6, D_089E6E14, -1, self->work->messages, mode,
                  self->work->layout.entries[0xD], 0, none, (int)self->work->field_A24);
}
