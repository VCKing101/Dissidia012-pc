#include "battle/agent11_types.h"

extern "C" {

extern int func_08A0AB88(BattleTask *self);
extern int func_08A0ABE0(BattleTask *self);
extern int func_08A0AD30(BattleTask *self);
extern int func_08A0AD50(BattleTask *self);
extern int func_08A0ADA8(BattleTask *self);
extern int func_08A0AE10(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_08A0AAD4(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_08A0AB88(self);
    case 1:
        return func_08A0ABE0(self);
    case 2:
        return func_08A0AD30(self);
    case 3:
        return func_08A0AD50(self);
    case 4:
        return func_08A0ADA8(self);
    case 5:
        return func_08A0AE10(self);
    }
    return 0;
}

}
