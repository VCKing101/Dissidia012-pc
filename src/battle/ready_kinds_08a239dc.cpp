#include "battle/agent31_types.h"

extern A31Battle *D_089D69F8;
extern "C" int func_08A07504(void *owner);
extern "C" unsigned char func_08ADFDDC(void *model);

extern "C" int func_08A239DC(ReadyScreen *self)
{
    if (func_08A07504(self->owner) == 1) {
        return -1;
    }
    A31Battle *battle = D_089D69F8;
    for (int side = 0; side < 2; side++) {
        self->prev_kinds[side] = self->kinds[side];
        A31Fighter *fighter = battle->fighters[side];
        if (fighter == 0) {
            return -2;
        }
        self->kinds[side] = func_08ADFDDC(fighter->model);
    }
    return 0;
}
