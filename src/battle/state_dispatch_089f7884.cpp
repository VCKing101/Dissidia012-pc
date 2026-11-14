#include "battle/agent11_types.h"

extern "C" {

extern int func_089F7938(BattleTask *self);
extern int func_089F7990(BattleTask *self);
extern int func_089F7A4C(BattleTask *self);
extern int func_089F7A9C(BattleTask *self);
extern int func_089F7AF4(BattleTask *self);
extern int func_089F7B5C(BattleTask *self);

/* Runs the handler for the task's current state. */
int func_089F7884(BattleTask *self)
{
    switch (self->state) {
    case 0:
        return func_089F7938(self);
    case 1:
        return func_089F7990(self);
    case 2:
        return func_089F7A4C(self);
    case 3:
        return func_089F7A9C(self);
    case 4:
        return func_089F7AF4(self);
    case 5:
        return func_089F7B5C(self);
    }
    return 0;
}

}
