#include "battle/auto_region5.h"

/* PartF6A34: starts hiding: plays motion 2 on field_160, hides every handle
 * and moves to phase 3. */
void func_08AF70CC(PartF6A34 *self)
{
    int i;

    func_08AF3D40(&self->field_160);
    for (i = 0; i < 0x42; i++) {
        if (self->handles[i] != 0) {
            func_088D9CFC(self->handles[i]);
        }
    }
    self->phase = 3;
}
