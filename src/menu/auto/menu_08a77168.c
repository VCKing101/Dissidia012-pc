#include "menu/auto_region3.h"

/* Btn77168: plays the enter motions. */
void func_08A77168(Btn77168 *self)
{
    func_088D2F68(&self->field_00, 0xB);
    func_088D2FA0(&self->field_00, 0xC);
    func_088D2F68(&self->field_04, 0);
    func_088D2FA0(&self->field_04, 1);
}
