#include "battle/agent11_types.h"

extern "C" {

extern int func_089F9630(BattleTask *self, float dt);
extern int func_089F968C(BattleTask *self, float dt);
extern int func_089F96E4(BattleTask *self, float dt);
extern int func_089F9704(BattleTask *self, float dt);
extern int func_089F9784(BattleTask *self, float dt);

int func_089F9594(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_089F9630(self, dt);
    case 1:
        return func_089F968C(self, dt);
    case 2:
        return func_089F96E4(self, dt);
    case 3:
        return func_089F9704(self, dt);
    case 4:
        return func_089F9784(self, dt);
    }
    return 0;
}

}
