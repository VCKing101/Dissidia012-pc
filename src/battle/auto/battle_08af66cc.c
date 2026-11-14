#include "battle/auto_region5.h"

/* PartF66C0: starts hiding: plays motion 2 on field_74, hides field_10 and
 * moves to phase 3. */
void func_08AF66CC(PartF66C0 *self)
{
    func_08AF3D40(&self->field_74);
    if (self->field_10 != 0) {
        func_088D9CFC(self->field_10);
    }
    self->phase = 3;
}
