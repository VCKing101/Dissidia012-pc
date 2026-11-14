#include "battle/agent11_types.h"

extern "C" {

extern int func_089F19D4(BattleTask *self, float x);
extern int func_089F1A2C(BattleTask *self, float y);
extern int func_089F1A74(BattleTask *self, float y);
extern int func_089F1ADC(BattleTask *self, float y);

/* Runs the handler for the current state; state 0 takes x, the others y. */
int func_089F1940(BattleTask *self, float x, float y)
{
    switch (self->state) {
    case 0:
        return func_089F19D4(self, x);
    case 1:
        return func_089F1A2C(self, y);
    case 2:
        return func_089F1A74(self, y);
    case 3:
        return func_089F1ADC(self, y);
    }
    return 0;
}

}
