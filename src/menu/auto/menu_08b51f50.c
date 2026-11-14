#include "menu/auto_region4.h"

/* Obj516E4: stops every part and the network screen. */
void func_08B51F50(Obj516E4 *self)
{
    func_088DE7B0(self->field_1C);
    func_088CDB04(self->field_33C);
    func_088D7574(self->field_C0);
    func_088CE7F4(self->field_380);
    func_08B3C944(D_08B41CC0, 1);
    func_088A28E8(0, 0);
    func_088965F4(D_089D6AE8, 0);
    func_088A3F2C(0);
    func_08864DB8();
    self->field_11 = 0;
    self->state = 0;
}
