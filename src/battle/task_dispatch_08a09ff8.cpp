#include "battle/agent11_types.h"

extern "C" {

extern int func_08A0A094(BattleTask *self, float dt);
extern int func_08A0A0F0(BattleTask *self, float dt);
extern int func_08A0A148(BattleTask *self, float dt);
extern int func_08A0A168(BattleTask *self, float dt);
extern int func_08A0A1E8(BattleTask *self, float dt);

int func_08A09FF8(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_08A0A094(self, dt);
    case 1:
        return func_08A0A0F0(self, dt);
    case 2:
        return func_08A0A148(self, dt);
    case 3:
        return func_08A0A168(self, dt);
    case 4:
        return func_08A0A1E8(self, dt);
    }
    return 0;
}

}
