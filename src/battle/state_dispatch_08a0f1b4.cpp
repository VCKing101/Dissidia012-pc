#include "battle/agent11_types.h"

extern "C" {

extern int func_08A0F268(BattleTask *self);
extern int func_08A0F354(BattleTask *self);
extern int func_08A0F3B4(BattleTask *self);
extern int func_08A0F414(BattleTask *self);
extern int func_08A0F474(BattleTask *self);
extern int func_08A0F4D4(BattleTask *self);

/* Runs the handler for the task's current mode. */
int func_08A0F1B4(BattleTask *self)
{
    switch (self->field_04) {
    case 0:
        return func_08A0F268(self);
    case 1:
        return func_08A0F354(self);
    case 2:
        return func_08A0F3B4(self);
    case 3:
        return func_08A0F414(self);
    case 4:
        return func_08A0F474(self);
    case 5:
        return func_08A0F4D4(self);
    }
    return 0;
}

}
