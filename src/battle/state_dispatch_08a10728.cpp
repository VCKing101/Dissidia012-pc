#include "battle/agent11_types.h"

extern "C" {

extern int func_08A107E4(BattleTask *self, float x);
extern int func_08A1084C(BattleTask *self, float y);
extern int func_08A108AC(BattleTask *self, float y);
extern int func_08A10954(BattleTask *self, float y);
extern int func_08A10994(BattleTask *self, float x);
extern int func_08A109EC(BattleTask *self, float y);

/* Runs the handler for the task's current state, passing it x or y. */
int func_08A10728(BattleTask *self, float x, float y)
{
    switch (self->state) {
    case 0:
        return func_08A107E4(self, x);
    case 1:
        return func_08A1084C(self, y);
    case 2:
        return func_08A108AC(self, y);
    case 3:
        return func_08A10954(self, y);
    case 4:
        return func_08A10994(self, x);
    case 5:
        return func_08A109EC(self, y);
    }
    return 0;
}

}
