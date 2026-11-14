#include "main/auto_region1.h"

/* Calls func_08809CEC on every object of the world's lists at +0x14, +0x24
 * and +0x34. */
void func_08809228(void *self)
{
    Fighter14 *fighter;
    Obj24 *object24;
    Obj34 *object34;

    fighter = 0;
    object24 = 0;
    object34 = 0;
    while (fighter = func_08841F84(fighter), fighter != 0) {
        func_08809CEC(self, fighter);
    }
    while (object24 = func_08842068(object24), object24 != 0) {
        func_08809CEC(self, object24);
    }
    while (object34 = func_088420A8(object34), object34 != 0) {
        func_08809CEC(self, object34);
    }
}
