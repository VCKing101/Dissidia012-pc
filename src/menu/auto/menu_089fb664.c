#include "menu/auto_region1.h"

extern const short D_08A0A79E[];

/* Screen0B6A0: calls func_088FD928 on D_089D7F90 with message 0x3489 and
 * layout entry 0x46. */
void func_089FB664(Screen0B6A0 *self)
{
    func_088FD928(D_089D7F90, self->heap, 2, 0xE, D_08A0A79E, 0x3489, self->work->messages, 4,
                  self->work->layout.entries[0x46], 0, 0, 0);
}
