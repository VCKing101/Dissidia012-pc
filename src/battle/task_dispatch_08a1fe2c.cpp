#include "battle/agent11_types.h"

extern "C" {

extern int func_08A1FEC8(BattleTask *self, float dt);
extern int func_08A1FF18(BattleTask *self, float dt);
extern int func_08A1FF88(BattleTask *self, float dt);
extern int func_08A1FFC8(BattleTask *self, float dt);
extern int func_08A20020(BattleTask *self, float dt);

int func_08A1FE2C(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_08A1FEC8(self, dt);
    case 1:
        return func_08A1FF18(self, dt);
    case 2:
        return func_08A1FF88(self, dt);
    case 3:
        return func_08A1FFC8(self, dt);
    case 4:
        return func_08A20020(self, dt);
    }
    return 0;
}

}
