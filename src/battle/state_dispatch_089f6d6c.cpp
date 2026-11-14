#include "battle/agent11_types.h"

extern "C" {

extern int func_089F6E20(BattleTask *self);
extern int func_089F6E70(BattleTask *self);
extern int func_089F6F2C(BattleTask *self);
extern int func_089F6F7C(BattleTask *self);
extern int func_089F6FCC(BattleTask *self);
extern int func_089F701C(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089F6D6C(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089F6E20(self);
    case 1:
        return func_089F6E70(self);
    case 2:
        return func_089F6F2C(self);
    case 3:
        return func_089F6F7C(self);
    case 4:
        return func_089F6FCC(self);
    case 5:
        return func_089F701C(self);
    }
    return 0;
}

}
