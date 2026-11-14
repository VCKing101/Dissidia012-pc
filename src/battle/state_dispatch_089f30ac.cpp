#include "battle/agent11_types.h"

extern "C" {

extern int func_089F3160(BattleTask *self);
extern int func_089F31B0(BattleTask *self);
extern int func_089F3270(BattleTask *self);
extern int func_089F3290(BattleTask *self);
extern int func_089F32E0(BattleTask *self);
extern int func_089F3330(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089F30AC(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089F3160(self);
    case 1:
        return func_089F31B0(self);
    case 2:
        return func_089F3270(self);
    case 3:
        return func_089F3290(self);
    case 4:
        return func_089F32E0(self);
    case 5:
        return func_089F3330(self);
    }
    return 0;
}

}
