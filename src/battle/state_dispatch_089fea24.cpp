#include "battle/agent11_types.h"

extern "C" {

extern int func_089FEAAC(BattleTask *self);
extern int func_089FEB14(BattleTask *self);
extern int func_089FEB74(BattleTask *self);
extern int func_089FEC54(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089FEA24(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089FEAAC(self);
    case 1:
        return func_089FEB14(self);
    case 2:
        return func_089FEB74(self);
    case 3:
        return func_089FEC54(self);
    }
    return 0;
}

}
