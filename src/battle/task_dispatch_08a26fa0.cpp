#include "battle/agent11_types.h"

extern "C" {

extern int func_08A2703C(BattleTask *self, float dt);
extern int func_08A27078(BattleTask *self, float dt);
extern int func_08A27134(BattleTask *self, float dt);
extern int func_08A27184(BattleTask *self, float dt);
extern int func_08A271DC(BattleTask *self, float dt);

int func_08A26FA0(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_08A2703C(self, dt);
    case 1:
        return func_08A27078(self, dt);
    case 2:
        return func_08A27134(self, dt);
    case 3:
        return func_08A27184(self, dt);
    case 4:
        return func_08A271DC(self, dt);
    }
    return 0;
}

}
