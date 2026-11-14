#include "menu/auto_region1.h"

extern const unsigned short D_08A09B4A[];
extern const unsigned short D_08A09B82[];

/* Screen0B6A0: plays D_08A09B4A, func_089FFFA8 with the low byte of
 * field_3158.field_04, then the text script D_08A09B82. */
void func_089F4554(Screen0B6A0 *self)
{
    func_088D3CF0(&self->work->layout, self->work->handles, D_08A09B4A);
    func_089FFFA8(self, 0, self->work->field_3158.field_04 & 0xFF, 0, 0);
    func_088D428C(self->work->handles, self->work->texts, D_08A09B82, 1);
}
