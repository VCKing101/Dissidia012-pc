#include "battle/agent27_types.h"

extern "C" {

struct DistVec {
    float x;
    float y;
    float z;
    float w;
};

void func_089A38FC(DistVec *out, float *a, float *b);
float func_089A3BF8(DistVec *v);

float func_08B2E734(ActorPos544 *self)
{
    ActorPos544 *target = (ActorPos544 *)self->slot_130();
    float zero = 0.0f;
    DistVec delta;

    if (target == 0) {
        return zero;
    }
    func_089A38FC(&delta, target->pos, self->pos);
    delta.y = zero;
    return func_089A3BF8(&delta);
}

}
