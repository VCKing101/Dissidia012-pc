#include "menu/auto_region4.h"

/* Panel26C88: refills field_29C when flag 2 is set. */
void func_08B26C88(Panel26C88 *self)
{
    if ((self->work->flags & 2) != 0) {
        func_08AF9CE0(D_08B01CF8, self->work->field_29C);
        self->work->flags = self->work->flags & 0xFFFFFFFD;
    }
}
