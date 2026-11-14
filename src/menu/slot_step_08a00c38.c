#include "menu/agent29_types.h"

/* Screen0B6A0: runs the slot page's current step: in mode 0 by the
   page's kind (+0x3158) and flags (+0x315C), in mode 1 the closing
   step. */
extern void func_089F4E28(Screen0B6A0 *self, int arg);
extern void func_089F4DB4(Screen0B6A0 *self);
extern void func_08A002E8(Screen0B6A0 *self);
extern void func_089F4554(Screen0B6A0 *self);
extern void func_089F46E8(Screen0B6A0 *self);
extern void func_089F4838(Screen0B6A0 *self);
extern void func_089F45C8(Screen0B6A0 *self);
extern void func_08A005D0(Screen0B6A0 *self);
extern void func_089F490C(Screen0B6A0 *self);
extern void func_089F4A4C(Screen0B6A0 *self, unsigned char kind);
extern void func_08A00968(Screen0B6A0 *self);
extern void func_089F4CD0(Screen0B6A0 *self);

void func_08A00C38(Screen0B6A0 *self)
{
    func_089F4E28(self, 0);
    if (WORK_SBYTE(self->work, 0x30D2) != 0) {
        if (WORK_SBYTE(self->work, 0x30D2) == 1) {
            func_089F4DB4(self);
        }
    } else {
        switch (WORK_BYTE(self->work, 0x3158)) {
        case 1:
            if (WORK_INT(self->work, 0x315C) & 0x800) {
                func_08A002E8(self);
            } else {
                func_089F4554(self);
            }
            break;
        case 2:
            if (WORK_INT(self->work, 0x315C) & 0x1000) {
                func_089F46E8(self);
            } else if (WORK_INT(self->work, 0x315C) & 0x2000) {
                func_089F4838(self);
            } else if (WORK_INT(self->work, 0x315C) & 0x10000) {
                func_089F45C8(self);
            } else {
                func_08A005D0(self);
            }
            break;
        case 3:
            func_089F490C(self);
            break;
        case 4:
        case 5:
            func_089F4A4C(self, WORK_BYTE(self->work, 0x3158));
            break;
        case 6:
            func_08A00968(self);
            break;
        case 7:
            func_089F4CD0(self);
            break;
        default:
            break;
        }
    }
}
