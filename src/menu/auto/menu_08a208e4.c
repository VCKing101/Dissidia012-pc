#include "menu/auto_region1.h"

/* Menu root: func_088F5FC0(9) on its messages, then fills the ten rows at
 * field_1E48 and keeps the count in field_1E44. */
void func_08A208E4(MenuRoot *self)
{
    RootWork *work;

    func_088F5FC0(self->work->messages, 9);
    work = self->work;
    self->work->field_1E44 = func_08929CCC(work->field_1E48, 10, work->messages, 9, work->field_2B90);
}
