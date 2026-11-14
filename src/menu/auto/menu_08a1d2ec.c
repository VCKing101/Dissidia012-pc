#include "menu/auto_region1.h"

/* Menu root: shows message id of kind (1 when kind is -1) through
 * func_088B1230. */
void func_08A1D2EC(MenuRoot *self, int id, int kind)
{
    char *text;

    if (kind == -1) {
        kind = 1;
    }
    text = func_0887B500(self->work->messages, kind, id);
    func_088B1230(D_089D727C, text, 0, 0, 0xA0, 0);
}
