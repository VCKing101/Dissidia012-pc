#include "battle/agent27_types.h"

extern "C" {

struct FighterRiseOps {
    static float y(float *pos)
    {
        return pos[1];
    }
};

float func_08B2E804(ActorPos544 *self)
{
    ActorPos544 *target = (ActorPos544 *)self->slot_130();

    if (target == 0) {
        return 0.0f;
    }
    float base = self->pos[1] - self->field_3C0;
    return FighterRiseOps::y(target->pos) - base;
}

}
