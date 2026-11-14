#include "battle/agent11_types.h"

extern "C" {

extern int func_08A1E974(BattleTask *self);
extern int func_08A1E9C4(BattleTask *self);
extern int func_08A1EA34(BattleTask *self);
extern int func_08A1EA74(BattleTask *self);
extern int func_08A1EAC4(BattleTask *self);
extern int func_08A1EB1C(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_08A1E8C0(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_08A1E974(self);
    case 1:
        return func_08A1E9C4(self);
    case 2:
        return func_08A1EA34(self);
    case 3:
        return func_08A1EA74(self);
    case 4:
        return func_08A1EAC4(self);
    case 5:
        return func_08A1EB1C(self);
    }
    return 0;
}

}
