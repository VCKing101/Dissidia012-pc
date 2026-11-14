#include "battle/agent11_types.h"

extern "C" {

extern int func_08A2A334(BattleTask *self, float dt);
extern int func_08A2A394(BattleTask *self, float dt);
extern int func_08A2A3E4(BattleTask *self, float dt);
extern int func_08A2A478(BattleTask *self, float dt);

int func_08A2A2AC(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        func_08A2A334(self, dt);
        break;
    case 1:
        func_08A2A394(self, dt);
        break;
    case 2:
        func_08A2A3E4(self, dt);
        break;
    case 3:
        func_08A2A478(self, dt);
        break;
    }
    return 0;
}

}
