#include "battle/auto_region5.h"

/* PartF4458: plays motion 9 on the handle at field_14 and sets its
 * field_0E to 0xB. */
void func_08AF57B8(PartF4458 *self)
{
    func_088D9F5C(self->field_14, 9, 1);
    self->field_14->field_0E = 0xB;
}
