#include "battle/auto_region3.h"

/* Calls func_08A97908 on what func_08AB326C returns for field_20. */
void func_08A641E8(Part64E48 *self)
{
    void *member;

    member = func_08AB326C(self->field_20);
    func_08A97908(member);
}
