#include "battle/agent11_types.h"

extern "C" {

extern int func_08A26698(BattleTask *self, float dt);
extern int func_08A26728(BattleTask *self, float dt);
extern int func_08A267E8(BattleTask *self, float dt);
extern int func_08A26830(BattleTask *self, float dt);
extern int func_08A26888(BattleTask *self, float dt);

int func_08A265FC(BattleTask *self, float dt)
{
    switch (self->field_04) {
    case 0:
        return func_08A26698(self, dt);
    case 1:
        return func_08A26728(self, dt);
    case 2:
        return func_08A267E8(self, dt);
    case 3:
        return func_08A26830(self, dt);
    case 4:
        return func_08A26888(self, dt);
    }
    return 0;
}

}
