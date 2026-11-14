#include "battle/agent11_types.h"

extern "C" {

extern int func_08A003E0(BattleTask *self, float x);
extern int func_08A00440(BattleTask *self, float x);
extern int func_08A004BC(BattleTask *self, float y);
extern int func_08A00514(BattleTask *self, float y);
extern int func_08A005A8(BattleTask *self, float y);

/* Runs the handler for the task's current state, passing it x or y. */
int func_08A0033C(BattleTask *self, float x, float y)
{
    switch (self->state) {
    case 0:
        return func_08A003E0(self, x);
    case 1:
        return func_08A00440(self, x);
    case 2:
        return func_08A004BC(self, y);
    case 3:
        return func_08A00514(self, y);
    case 4:
        return func_08A005A8(self, y);
    }
    return 0;
}

}
