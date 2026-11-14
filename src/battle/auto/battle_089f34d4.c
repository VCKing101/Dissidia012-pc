#include "battle/auto_region1.h"

/* PartF3820 release: the member, the resource at field_04, then a reset. */
void func_089F34D4(PartF3820 *self)
{
    func_089F7D0C(&self->field_08);
    if (self->field_04) {
        func_088DA060(self->field_04);
        self->field_04 = 0;
    }
    func_089F34BC(self);
}
