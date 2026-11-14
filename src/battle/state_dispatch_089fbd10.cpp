#include "battle/agent11_types.h"

extern "C" {

extern int func_089FBD98(BattleTask *self);
extern int func_089FBDFC(BattleTask *self);
extern int func_089FBE1C(BattleTask *self);
extern int func_089FBE74(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089FBD10(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089FBD98(self);
    case 1:
        return func_089FBDFC(self);
    case 2:
        return func_089FBE1C(self);
    case 3:
        return func_089FBE74(self);
    }
    return 0;
}

}
