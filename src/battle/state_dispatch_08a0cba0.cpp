#include "battle/agent11_types.h"

extern "C" {

extern int func_08A0CC28(BattleTask *self);
extern int func_08A0CC88(BattleTask *self);
extern int func_08A0CCD8(BattleTask *self);
extern int func_08A0CDDC(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_08A0CBA0(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_08A0CC28(self);
    case 1:
        return func_08A0CC88(self);
    case 2:
        return func_08A0CCD8(self);
    case 3:
        return func_08A0CDDC(self);
    }
    return 0;
}

}
