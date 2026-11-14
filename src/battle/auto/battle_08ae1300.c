#include "battle/auto_region4.h"

/* PartE7B50: once field_2A8 is idle, calls func_08889F70/func_08889FB8 for
 * the fighter and func_08ADFCC4; always clears fighter flag 0x800000. */
void func_08AE1300(PartE7B50 *self)
{
    int busy;
    BattleFighter4 *fighter;

    busy = func_08ADFDDC(self->field_2A8);
    fighter = self->fighter;
    if (busy == 0) {
        func_08889F70(fighter->field_10CC, 0);
        func_08889FB8(self->fighter->field_10CC, 0, 0);
        func_08ADFCC4(self->field_2A8);
        fighter = self->fighter;
    }
    fighter->flags &= ~0x800000;
}
