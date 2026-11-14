#include "menu/agent29_types.h"

/* Screen0B6A0: while the count-up counter (+0x31FC) runs, moves the
   record gauge (model 0xC) to the frame for the running count (out of
   10000); once it has run out, keeps the count (+0x32B4) in the record,
   stops the count sound (+0x320C) and, once the wait (+0x31E8) is over,
   moves on. */
extern int func_089F2758(Screen0B6A0 *self, void *counter);
extern int func_089F26E0(Screen0B6A0 *self, void *counter);
extern int func_088DA024(MenuModel *model);
extern void func_088D9AEC(MenuModel *model, float frame);
extern void func_089F80F8(Screen0B6A0 *self);

void func_089F2D48(Screen0B6A0 *self)
{
    int count;
    int frames;

    if (func_089F2758(self, &WORK_BYTE(self->work, 0x31FC)) != 0) {
        count = WORK_INT(self->work, 0x32B4) + func_089F26E0(self, &WORK_BYTE(self->work, 0x31FC));
        frames = func_088DA024(self->work->handles[0xC]);
        func_088D9AEC(self->work->handles[0xC], (int)(frames * (unsigned int)count) / 10000);
        self->work->handles[0xC]->field_20 &= ~2;
        if (func_089F2758(self, &WORK_BYTE(self->work, 0x31FC)) == 0) {
            Record8CB164 *record;

            WORK_INT(self->work, 0x32B4) = count;
            record = func_088CB164(D_089D712C->field_F58, WORK_INT(self->work, 0x328C));
            record->field_06 = WORK_INT(self->work, 0x32B4);
            if (WORK_INT(self->work, 0x320C) != -1) {
                func_088D4BF4(WORK_INT(self->work, 0x320C));
                WORK_INT(self->work, 0x320C) = -1;
            }
            if (func_089F2758(self, &WORK_BYTE(self->work, 0x31E8)) == 0) {
                func_089F80F8(self);
            }
        }
    }
}
