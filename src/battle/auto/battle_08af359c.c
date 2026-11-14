#include "battle/auto_region4.h"

/* PartF2CBC: calls func_08AF62F8 on field_24 and sets field_73. */
void func_08AF359C(PartF2CBC *self)
{
    func_08AF62F8(self->field_24);
    self->field_73 = 1;
}
