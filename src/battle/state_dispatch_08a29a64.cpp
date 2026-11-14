#include "battle/agent11_types.h"

extern "C" {

extern int func_08A29B08(BattleTask *self, float x);
extern int func_08A29B68(BattleTask *self, float x);
extern int func_08A29BD8(BattleTask *self, float y);
extern int func_08A29C30(BattleTask *self, float y);
extern int func_08A29CC4(BattleTask *self, float y);

/* Runs the handler for the task's current state, passing it x or y. */
int func_08A29A64(BattleTask *self, float x, float y)
{
    switch (self->state) {
    case 0:
        return func_08A29B08(self, x);
    case 1:
        return func_08A29B68(self, x);
    case 2:
        return func_08A29BD8(self, y);
    case 3:
        return func_08A29C30(self, y);
    case 4:
        return func_08A29CC4(self, y);
    }
    return 0;
}

}
