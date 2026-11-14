#include "main/auto_region2.h"

/* Calls func_0884B908 on the part of every +0x14 and +0x24 world object, inside state 0x15. */
void func_0884BE74(unsigned int arg, unsigned char on)
{
    Fighter14 *fighter;
    Obj24 *object;

    fighter = 0;
    object = 0;
    func_089A0384(0x15);
    while (fighter = func_08841F84(fighter), fighter != 0) {
        func_0884B908(fighter->field_76C, on);
    }
    while (object = func_08842068(object), object != 0) {
        func_0884B908(object->field_5D8, on);
    }
    func_089A03DC(0x15);
}
