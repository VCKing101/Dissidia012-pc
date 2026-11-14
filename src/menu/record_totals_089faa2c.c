#include "menu/agent29_types.h"

/* Screen0B6A0: refreshes the pull page's totals (+0x3298, +0x329C) and
   the points (+0x32C8), next points (+0x32A0) and count (+0x32B4) of the
   five records, redrawing each record that is set. */
extern int func_089FC2DC(Screen0B6A0 *self);
extern unsigned char func_089FC378(Screen0B6A0 *self);
extern int func_089FA6E0(Screen0B6A0 *self, int record, int arg);
extern int func_089FA83C(Screen0B6A0 *self, int record, int points);
extern int func_089FA884(Screen0B6A0 *self, int record);
extern void func_089FA8AC(Screen0B6A0 *self, int index);

void func_089FAA2C(Screen0B6A0 *self)
{
    int i;
    int id;

    WORK_INT(self->work, 0x3298) = func_089FC2DC(self);
    WORK_BYTE(self->work, 0x329C) = func_089FC378(self);
    for (i = 0; i < 5; i++) {
        id = self->work->field_2E84[i];
        if (id != 0) {
            (&WORK_INT(self->work, 0x32C8))[i] = func_089FA6E0(self, id, 0);
            if ((&WORK_INT(self->work, 0x2E98))[i] & 1) {
                (&WORK_INT(self->work, 0x32A0))[i] = 0;
                (&WORK_INT(self->work, 0x32B4))[i] = 0;
            } else {
                (&WORK_INT(self->work, 0x32A0))[i] = func_089FA83C(self, id, (&WORK_INT(self->work, 0x32C8))[i]);
                (&WORK_INT(self->work, 0x32B4))[i] = func_089FA884(self, id);
            }
            func_089FA8AC(self, i);
        } else {
            (&WORK_INT(self->work, 0x32C8))[i] = 0;
            (&WORK_INT(self->work, 0x32A0))[i] = 0;
            (&WORK_INT(self->work, 0x32B4))[i] = 0;
        }
    }
}
