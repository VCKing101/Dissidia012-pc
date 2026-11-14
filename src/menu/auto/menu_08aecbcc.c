#include "menu/auto_region3.h"

/* Screen23F0: applies field_19B8, redraws and resets the cursor. */
void func_08AECBCC(Screen23F0 *self)
{
    func_08AEC700(self, self->work->field_19B8);
    func_08AECD10(self);
    func_088D4788(&self->work->cursor, 0, 0);
}
