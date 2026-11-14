#include "menu/agent29_types.h"

/* Screen0B6A0: changes the pull count (+0x2734) by `step` (kept in
   0..999) and sets up the counter animation: the change (+0x3214), its
   frames (+0x3218: |change|, or |change| / 9 + 10 from 10 on) and the
   sound (+0x321C); a gain is added to the total (+0x27A8). */
extern int func_0893486C(int value);

void func_089F31CC(Screen0B6A0 *self, short step)
{
    unsigned int count = WORK_U16(self->work, 0x2734);
    int change = count + step;
    short frames;

    if (change < 0) {
        change = 0;
    } else if (change >= 1000) {
        change = 999;
    }
    change -= count;
    WORK_INT(self->work, 0x3214) = change;
    if (change != 0) {
        if (func_0893486C(change) >= 10) {
            frames = func_0893486C(change) / 9 + 10;
        } else {
            frames = func_0893486C(change);
        }
        WORK_SHORT(self->work, 0x3218) = frames;
        WORK_INT(self->work, 0x321C) = func_088D4BF4(0x15);
    } else {
        WORK_SHORT(self->work, 0x3218) = 1;
    }
    WORK_SHORT(self->work, 0x321A) = 0;
    if (change > 0) {
        WORK_INT(self->work, 0x27A8) += change;
    }
    func_089F3498(self, 0);
}
