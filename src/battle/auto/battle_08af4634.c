#include "battle/auto_region4.h"

/* PartF4458: plays motion 0xC on the handle at field_14 and sets its
 * field_0E to 0x10. */
void func_08AF4634(PartF4458 *self)
{
    func_088D9F5C(self->field_14, 0xC, 1);
    self->field_14->field_0E = 0x10;
}
