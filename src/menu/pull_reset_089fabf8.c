#include "menu/agent29_types.h"

/* Screen0B6A0: starts a new pull: takes the current record id from
   D_09B6DA40, clears the record and count tables and the slot state,
   resets that record and redraws. */
typedef struct PullState29 {
    int field_00;
    int record;
} PullState29;

extern PullState29 D_09B6DA40;
extern void func_089FA91C(Screen0B6A0 *self);
extern void func_089FAA2C(Screen0B6A0 *self);

void func_089FABF8(Screen0B6A0 *self)
{
    Record8CB164 *record;
    int i;

    WORK_INT(self->work, 0x328C) = D_09B6DA40.record;
    for (i = 0; i < 5; i++) {
        self->work->field_2E84[i] = 0;
        (&WORK_INT(self->work, 0x2E98))[i] = 0;
    }
    for (i = 0; i < 0x23; i++) {
        (&WORK_INT(self->work, 0x2EAC))[i] = 0;
    }
    WORK_INT(self->work, 0x2E80) = 0;
    self->work->field_2E84[0] = WORK_INT(self->work, 0x328C);
    record = func_088CB164(D_089D712C->field_F58, WORK_INT(self->work, 0x328C));
    record->field_0C = 0;
    record->field_06 = 0;
    func_089FA91C(self);
    func_089FAA2C(self);
}
