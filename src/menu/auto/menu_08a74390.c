#include "menu/auto_region3.h"

/* Btn74390: plays the enter motions. */
void func_08A74390(Btn74390 *self)
{
    func_088D2F68(&self->field_00, 0xE);
    func_088D2FA0(&self->field_00, 0xF);
    func_088D2F68(&self->field_08, 0);
    func_088D2FA0(&self->field_08, 1);
}
