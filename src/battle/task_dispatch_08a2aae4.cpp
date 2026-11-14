#include "battle/agent11_types.h"

extern "C" {

extern int func_08A2AB4C(BattleTask *self, float dt);
extern int func_08A2ABC4(BattleTask *self, float dt);
extern int func_08A2AC00(BattleTask *self, float dt);

int func_08A2AAE4(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_08A2AB4C(self, dt);
    case 1:
        return func_08A2ABC4(self, dt);
    case 2:
        return func_08A2AC00(self, dt);
    }
    return 0;
}

}
