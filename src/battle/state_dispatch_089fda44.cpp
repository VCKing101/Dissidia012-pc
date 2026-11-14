#include "battle/agent11_types.h"

extern "C" {

extern int func_089FDACC(BattleTask *self);
extern int func_089FDB2C(BattleTask *self);
extern int func_089FDB84(BattleTask *self);
extern int func_089FDC64(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089FDA44(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089FDACC(self);
    case 1:
        return func_089FDB2C(self);
    case 2:
        return func_089FDB84(self);
    case 3:
        return func_089FDC64(self);
    }
    return 0;
}

}
