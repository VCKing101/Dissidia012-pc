#include "battle/auto_region3.h"

/* Calls func_08AB29B0 on what func_08AB327C returns for field_20. */
void func_08A64360(Part64E48 *self)
{
    void *member;

    member = func_08AB327C(self->field_20);
    func_08AB29B0(member);
}
