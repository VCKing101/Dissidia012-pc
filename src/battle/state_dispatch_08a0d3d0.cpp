#include "battle/agent11_types.h"

extern "C" {

extern int func_08A0D458(BattleTask *self);
extern int func_08A0D4B8(BattleTask *self);
extern int func_08A0D508(BattleTask *self);
extern int func_08A0D5C4(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_08A0D3D0(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_08A0D458(self);
    case 1:
        return func_08A0D4B8(self);
    case 2:
        return func_08A0D508(self);
    case 3:
        return func_08A0D5C4(self);
    }
    return 0;
}

}
