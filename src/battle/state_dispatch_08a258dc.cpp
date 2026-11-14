#include "battle/agent11_types.h"

extern "C" {

extern int func_08A25964(BattleTask *self);
extern int func_08A259C4(BattleTask *self);
extern int func_08A25A34(BattleTask *self);
extern int func_08A25A98(BattleTask *self);

/* Runs the handler for the task's current mode. */
int func_08A258DC(BattleTask *self)
{
    switch (self->field_04) {
    case 0:
        return func_08A25964(self);
    case 1:
        return func_08A259C4(self);
    case 2:
        return func_08A25A34(self);
    case 3:
        return func_08A25A98(self);
    }
    return 0;
}

}
