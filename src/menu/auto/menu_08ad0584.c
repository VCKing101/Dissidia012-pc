#include "menu/auto_region3.h"

extern const unsigned short D_08AD9214[];
extern const unsigned short D_08AD92B4[];

/* ScreenDB30: plays D_08AD9214 and its texts; hides handles 0xE2 + field_CACC .. 0xE8. */
void func_08AD0584(ScreenDB30 *self)
{
    int i;

    func_08AD00F8(self);
    func_088D3CF0(&self->work->layout, self->work->handles, D_08AD9214);
    func_088D428C(self->work->handles, self->work->texts, D_08AD92B4, 1);
    for (i = self->work->field_CACC; i < 7; i++) {
        func_088D9CFC(self->work->handles[i + 0xE2]);
    }
}
