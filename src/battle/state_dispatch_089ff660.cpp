#include "battle/agent11_types.h"

extern "C" {

extern int func_089FF714(BattleTask *self);
extern int func_089FF764(BattleTask *self);
extern int func_089FF7BC(BattleTask *self);
extern int func_089FF838(BattleTask *self);
extern int func_089FF930(BattleTask *self);
extern int func_089FFA84(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089FF660(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089FF714(self);
    case 1:
        return func_089FF764(self);
    case 2:
        return func_089FF7BC(self);
    case 3:
        return func_089FF838(self);
    case 4:
        return func_089FF930(self);
    case 5:
        return func_089FFA84(self);
    }
    return 0;
}

}
