#include "menu/agent29_types.h"

/* Screen0B6A0: uses (use set) or sets the current entry's item (+0x01):
   when the item can be taken, takes it and shows message 4; otherwise,
   when it can be set, sets it and shows message 3 (and message 0xE with
   step 9 when that filled the slots). Gives whether anything happened;
   then redraws the record board. */
extern int func_089FC478(Screen0B6A0 *self, int item);
extern void func_089FC42C(Screen0B6A0 *self, int item);
extern int func_089FC260(Screen0B6A0 *self);
extern int func_089FC0A0(Screen0B6A0 *self, int item);
extern void func_089FAF14(Screen0B6A0 *self, int message, short item);

int func_089F7FA4(Screen0B6A0 *self, unsigned char use)
{
    int done = 0;
    int current = func_089F2FF0(self);
    int item = self->work->entries[current].field_01;
    int full;

    if (use) {
        if (func_089FC478(self, item) == 0) {
            func_089FC42C(self, item);
            func_089FAF14(self, 4, item);
            done = 1;
            WORK_INT(self->work, 0x3444) = 0;
        }
    } else {
        if (func_089FC260(self) == 0) {
            full = func_089FC0A0(self, item);
            func_089FAF14(self, 3, item);
            if (full) {
                func_089FAF14(self, 0xE, -1);
                WORK_INT(self->work, 0x3444) = 9;
            } else {
                WORK_INT(self->work, 0x3444) = 0;
            }
            done = 1;
        }
    }
    if (done) {
        WORK_INT(self->work, 0x3448) = 0;
        WORK_INT(self->work, 0x344C) = 0;
    }
    func_089FC91C(self);
    return done;
}
