#include "battle/auto_region4.h"

/* PartF4458: plays motion 4 on the handle at field_14 and sets its
 * field_0E to 6. */
void func_08AF4CDC(PartF4458 *self)
{
    func_088D9F5C(self->field_14, 4, 1);
    self->field_14->field_0E = 6;
}
