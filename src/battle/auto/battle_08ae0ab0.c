#include "battle/auto_region4.h"

/* PartE7B50: copies field_14 of the fighter's field_52C into field_1C,
 * then calls func_08AE9978 on the list. */
void func_08AE0AB0(PartE7B50 *self)
{
    self->field_1C = self->fighter->field_52C->field_14;
    func_08AE9978(&self->field_30);
}
