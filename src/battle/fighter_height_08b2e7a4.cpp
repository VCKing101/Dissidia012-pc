#include "battle/agent27_types.h"

extern "C" {

struct FighterHeightOps {
    static float y(float *pos)
    {
        return pos[1];
    }
};

float func_08B2E7A4(ActorPos544 *self)
{
    ActorPos544 *target = (ActorPos544 *)self->slot_130();

    if (target == 0) {
        return 0.0f;
    }
    return FighterHeightOps::y(target->pos) - FighterHeightOps::y(self->pos);
}

}
