#include "menu/auto_region2.h"

/* State D_08A6DAD0: on arg 0 requests kind 3 (field_04 == 1) or 5. */
void func_08A6DB50(State6D7E8 *self, Obj6ECF4 *owner, int arg)
{
    if (arg == 0) {
        if (self->field_04 == 1) {
            func_08A6E2CC(&owner->request, 3, 0, 0);
        } else {
            func_08A6E2CC(&owner->request, 5, 0, 0);
        }
    }
}
