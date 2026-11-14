#include "menu/auto_region2.h"

/* Panel6C638: plays motion 5 on handle 0x15 and holds it (flag 0x1000, field_0E 4). */
void func_08A6C3A0(Panel6C638 *self)
{
    func_088D9F5C(self->handles[0x15], 5, 1);
    self->handles[0x15]->field_20 = self->handles[0x15]->field_20 | 0x1000;
    ((MenuModelView *)self->handles[0x15])->field_0E = 4;
}
