#include "menu/auto_region1.h"

/* Menu root: points field_6914 at field_67B4, installs the callbacks
 * func_08A206B0 / func_08A206D0, clears field_68E0 and registers field_6850
 * with D_08B59230. */
void func_08A1E240(MenuRoot *self)
{
    self->work->field_6914 = &self->work->field_67B4;
    self->work->field_6920 = func_08A206B0;
    self->work->field_6924 = func_08A206D0;
    func_08935828(self->work->field_68E0, 0, sizeof(self->work->field_68E0));
    func_08B58B64(D_08B59230, &self->work->field_6850);
}
