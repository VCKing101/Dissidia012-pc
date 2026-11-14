#include "menu/agent29_types.h"

/* Screen0B6A0: while the level gauge is open (page flag 2), plays its
   script and, while its counter (+0x3250) runs, shows the counter's
   digits on the three digit models (0xF7..) with the digit motions of
   D_08A0B43C (+0x31E4 counts the extra digits); once the counter is
   done, shows the gauge (page flag 0x8000) and moves on. */
extern const short D_08A0A212[];
extern const unsigned short D_08A0B43C[];
extern int func_089F2758(Screen0B6A0 *self, void *counter);
extern int func_089F26E0(Screen0B6A0 *self, void *counter);
extern void func_08A03D48(Screen0B6A0 *self);

void func_089F8E18(Screen0B6A0 *self)
{
    int value;
    int i;

    if ((WORK_INT(self->work, 0x3238) & 2) == 0) {
        return;
    }
    func_088D3F00(&self->work->layout, self->work->handles, D_08A0A212);
    if (func_089F2758(self, &WORK_BYTE(self->work, 0x3250)) != 0) {
    value = func_089F26E0(self, &WORK_BYTE(self->work, 0x3250));
    WORK_BYTE(self->work, 0x31E4) = 0;
    for (i = 0; i < 3; i++) {
        unsigned char digit = value % 10;
        func_088D9F5C(self->work->handles[0xF7 + i], D_08A0B43C[digit], 1);
        value /= 10;
        if (value == 0) {
            break;
        }
        WORK_BYTE(self->work, 0x31E4)++;
    }
    if (func_089F2758(self, &WORK_BYTE(self->work, 0x3250)) == 0) {
        if (self->work->flags & 0x8000) {
            func_088D9CD8(self->work->handles[0xFA]);
        }
        func_08A03D48(self);
    }
    }
}
