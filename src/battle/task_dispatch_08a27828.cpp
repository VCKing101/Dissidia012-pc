#include "battle/agent11_types.h"

extern "C" {

extern int func_08A278C4(BattleTask *self, float dt);
extern int func_08A278F8(BattleTask *self, float dt);
extern int func_08A27934(BattleTask *self, float dt);
extern int func_08A27954(BattleTask *self, float dt);
extern int func_08A279AC(BattleTask *self, float dt);

int func_08A27828(BattleTask *self, float dt)
{
    switch (self->state) {
    case 0:
        return func_08A278C4(self, dt);
    case 1:
        return func_08A278F8(self, dt);
    case 2:
        return func_08A27934(self, dt);
    case 3:
        return func_08A27954(self, dt);
    case 4:
        return func_08A279AC(self, dt);
    }
    return 0;
}

}
