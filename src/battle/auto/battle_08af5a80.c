#include "battle/auto_region5.h"

/* PartF5A80: shows its three handles, then runs func_08AF5AE8. */
void func_08AF5A80(PartF5A80 *self)
{
    func_088D9CD8(self->field_00);
    func_088D9CD8(self->field_04);
    func_088D9CD8(self->field_08);
    func_08AF5AE8(self);
}
