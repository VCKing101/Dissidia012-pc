#include "menu/agent29_types.h"

/* Screen0B6A0: writes pull kind `kind` back to its save record: the pulls
   left (+0x32C8 less +0x32A0) and the level (+0x32B4). */
void func_089FA8AC(Screen0B6A0 *self, int kind)
{
    Record8CB164 *record = func_088CB164(D_089D712C->field_F58, self->work->field_2E84[kind]);

    record->field_0C = (&WORK_INT(self->work, 0x32C8))[kind] - (&WORK_INT(self->work, 0x32A0))[kind];
    record->field_06 = (&WORK_INT(self->work, 0x32B4))[kind];
}
