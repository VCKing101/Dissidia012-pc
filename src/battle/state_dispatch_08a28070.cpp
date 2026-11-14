#include "battle/agent11_types.h"

extern "C" {

extern int func_08A280F8(BattleTask *self);
extern int func_08A28158(BattleTask *self);
extern int func_08A281B0(BattleTask *self);
extern int func_08A2832C(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_08A28070(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_08A280F8(self);
    case 1:
        return func_08A28158(self);
    case 2:
        return func_08A281B0(self);
    case 3:
        return func_08A2832C(self);
    }
    return 0;
}

}
