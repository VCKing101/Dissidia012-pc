#include "battle/agent31_types.h"

/* Moves the hit effect's current trails to the old pair (removing the old
 * ones) and spawns two new trails. */
struct TrailParts {
    unsigned char pad_00[0xE8];
    A31BoneEffect *trail_a;
    A31BoneEffect *trail_b;
    A31BoneEffect *old_a;
    A31BoneEffect *old_b;
};

struct TrailEffect {
    A31VActor *actor;
    unsigned char pad_04[0x10];
    TrailParts *parts;
};

extern float D_08A33584;

extern "C" {

extern void func_088A6C70(A31BoneEffect *effect);

void func_08A30BE0(TrailEffect *self)
{
    if (self->parts->old_a != 0) {
        func_088A6C70(self->parts->old_a);
        self->parts->old_a = 0;
    }
    if (self->parts->old_b != 0) {
        func_088A6C70(self->parts->old_b);
        self->parts->old_b = 0;
    }
    if (self->parts->trail_a != 0) {
        self->parts->old_a = self->parts->trail_a;
        self->parts->trail_a = 0;
    }
    if (self->parts->trail_b != 0) {
        self->parts->old_b = self->parts->trail_b;
        self->parts->trail_b = 0;
    }
    self->parts->trail_a = A31Spawn::at_hit(self->actor, 0x3D);
    self->parts->trail_b = A31Spawn::at_hit(self->actor, 0x55);
    if (self->parts->trail_a != 0) {
        self->parts->trail_a->scale = D_08A33584;
    }
    if (self->parts->trail_b != 0) {
        self->parts->trail_b->scale = D_08A33584;
    }
}

}
