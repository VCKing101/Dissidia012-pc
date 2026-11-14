#include "battle/agent11_types.h"

extern "C" {

extern int func_08A17A9C(BattleTask *self, float dt);
extern int func_08A17B2C(BattleTask *self, float dt);
extern int func_08A17B84(BattleTask *self, float dt);
extern int func_08A17BC4(BattleTask *self, float dt);
extern int func_08A17C50(BattleTask *self, float dt);

int func_08A17A00(BattleTask *self, float dt)
{
    switch (self->field_04) {
    case 0:
        return func_08A17A9C(self, dt);
    case 1:
        return func_08A17B2C(self, dt);
    case 2:
        return func_08A17B84(self, dt);
    case 3:
        return func_08A17BC4(self, dt);
    case 4:
        return func_08A17C50(self, dt);
    }
    return 0;
}

}
