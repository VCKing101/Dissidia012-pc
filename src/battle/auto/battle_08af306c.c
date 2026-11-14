#include "battle/auto_region4.h"

/* PartF2CBC: calls func_088EC658 on field_04 and sets field_71. */
void func_08AF306C(PartF2CBC *self)
{
    func_088EC658(self->field_04);
    self->field_71 = 1;
}
