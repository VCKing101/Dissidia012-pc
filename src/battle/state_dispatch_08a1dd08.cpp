#include "battle/agent11_types.h"

extern "C" {

extern int func_08A1DDBC(BattleTask *self);
extern int func_08A1DE0C(BattleTask *self);
extern int func_08A1DE64(BattleTask *self);
extern int func_08A1DEE0(BattleTask *self);
extern int func_08A1DF40(BattleTask *self);
extern int func_08A1E018(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_08A1DD08(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_08A1DDBC(self);
    case 1:
        return func_08A1DE0C(self);
    case 2:
        return func_08A1DE64(self);
    case 3:
        return func_08A1DEE0(self);
    case 4:
        return func_08A1DF40(self);
    case 5:
        return func_08A1E018(self);
    }
    return 0;
}

}
