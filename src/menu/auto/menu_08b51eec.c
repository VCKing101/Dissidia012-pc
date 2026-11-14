#include "menu/auto_region4.h"

/* Obj516E4: reads the result of field_394 into a 340-byte block and hands
 * it to ScreenB41CC0. */
void func_08B51EEC(Obj516E4 *self)
{
    char info[340];

    func_088CDA34(self->field_33C);
    func_08B520C8(self);
    func_0892A13C(info);
    func_0892F7D0(self->field_394, info, self->field_08, 0xD);
    func_08B3CA8C(D_08B41CC0, info);
    self->state = 1;
}
