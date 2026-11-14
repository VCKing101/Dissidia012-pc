#include "menu/auto_region4.h"

extern const unsigned short D_08B4C798[];

/* Widget4C010: makes its two handles from its model set. */
void func_08B4C500(Widget4C010 *self)
{
    MenuModel *model;
    int id;
    int i;

    func_08B4C580(self);
    for (i = 0; i < 2; i++) {
        model = func_088DAA68(self->id);
        id = self->id;
        self->handles[i] = model;
        func_088DAB7C(id, model);
    }
    func_088D3CF0(0, self->handles, D_08B4C798);
    func_088D9CFC(self->handles[1]);
}
