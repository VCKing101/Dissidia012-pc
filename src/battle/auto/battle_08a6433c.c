#include "battle/auto_region3.h"

/* Calls func_08AAE57C on what func_08AB327C returns for field_20. */
void func_08A6433C(Part64E48 *self)
{
    void *member;

    member = func_08AB327C(self->field_20);
    func_08AAE57C(member);
}
