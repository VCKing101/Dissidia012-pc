#include "battle/agent11_types.h"

extern "C" {

extern int func_089FB3F0(BattleTask *self, float dt);
extern int func_089FB440(BattleTask *self, float dt);
extern int func_089FB4B0(BattleTask *self, float dt);
extern int func_089FB4F0(BattleTask *self, float dt);
extern int func_089FB548(BattleTask *self, float dt);

int func_089FB354(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_089FB3F0(self, dt);
    case 1:
        return func_089FB440(self, dt);
    case 2:
        return func_089FB4B0(self, dt);
    case 3:
        return func_089FB4F0(self, dt);
    case 4:
        return func_089FB548(self, dt);
    }
    return 0;
}

}
