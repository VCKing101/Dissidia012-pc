#include "battle/auto_region5.h"

/* Hands `other`'s field_F8 member to the object func_08ABB4B4 returns for
 * D_08ABBDF8. */
void func_08B40A50(void *self, Part40A50 *other)
{
    PartB7ED0 *object;

    object = func_08ABB4B4(D_08ABBDF8);
    func_08AB7B7C(object, other->field_F8->field_80);
}
