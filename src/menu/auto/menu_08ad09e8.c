#include "menu/auto_region3.h"

/* ScreenDB30: func_088B745C on record field_1210 of D_08B01CF8's data, then two updates. */
void func_08AD09E8(ScreenDB30 *self)
{
    func_088B745C(&((ScreenB01CF8 *)D_08B01CF8)->work->records[self->work->field_1210]);
    func_08AF3ECC(D_08B01CF8);
    func_08AFD194(D_08B01CF8, 1, 0);
}
