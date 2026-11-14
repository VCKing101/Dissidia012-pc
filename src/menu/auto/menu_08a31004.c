#include "menu/auto_region1.h"

/* Screen35A00: shuts down and frees its work block. */
void func_08A31004(Screen35A00 *self)
{
    func_088A952C(D_089D6DC8);
    func_088A5E6C(D_089D6D80);
    func_08A31438(self);
    func_0885C538(D_089D5EC0->field_710, self->work->field_20, 0);
    func_08841ED8();
    func_088CB9C8();
    func_0885C538(self->heap, self->work->field_198, 0);
    func_0885A31C(D_089D4DC8);
    func_0885C538(self->heap, self->work, 0);
    self->work = 0;
}
