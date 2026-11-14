#include "menu/auto_region1.h"

/* Screen38EA8: when field_D58 is set, resets field_D54 and hands it
 * field_D58 back, then clears field_D58. */
void func_08A367CC(Screen38EA8 *self)
{
    Obj367CC *object;

    if (self->work->field_D58 != -1) {
        object = self->work->field_D54;
        func_0889F704(&object->field_04);
        object->field_30 = 0;
        func_088A3D14(self->work->field_D54, self->work->field_D58);
        self->work->field_D58 = -1;
    }
}
