#include "battle/agent11_types.h"

extern "C" {

extern int func_089FA080(BattleTask *self);
extern int func_089FA0E0(BattleTask *self);
extern int func_089FA154(BattleTask *self);
extern int func_089FA210(BattleTask *self);
extern int func_089FA250(BattleTask *self);
extern int func_089FA2A8(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089F9FCC(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089FA080(self);
    case 1:
        return func_089FA0E0(self);
    case 2:
        return func_089FA154(self);
    case 3:
        return func_089FA210(self);
    case 4:
        return func_089FA250(self);
    case 5:
        return func_089FA2A8(self);
    }
    return 0;
}

}
