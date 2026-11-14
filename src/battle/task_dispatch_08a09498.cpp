#include "battle/agent11_types.h"

extern "C" {

extern int func_08A09534(BattleTask *self, float dt);
extern int func_08A095E4(BattleTask *self, float dt);
extern int func_08A09644(BattleTask *self, float dt);
extern int func_08A09694(BattleTask *self, float dt);
extern int func_08A0976C(BattleTask *self, float dt);

int func_08A09498(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_08A09534(self, dt);
    case 1:
        return func_08A095E4(self, dt);
    case 2:
        return func_08A09644(self, dt);
    case 3:
        return func_08A09694(self, dt);
    case 4:
        return func_08A0976C(self, dt);
    }
    return 0;
}

}
