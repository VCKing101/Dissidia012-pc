#include "battle/agent31_types.h"

/* Finishes the hit effect: plays the end motions on the attacker and the
 * hit reaction on the target, then spawns the finish sparks (one per
 * level). */
struct FinishParts {
    unsigned char pad_00[2];
    unsigned char level;
    unsigned char pad_03[2];
    unsigned char stage;
};

struct FinishEffect {
    A31VActor *actor;
    A31VActor *target;
    unsigned char pad_08[0xC];
    FinishParts *parts;
};

extern unsigned char *D_089D4DC8;

extern "C" void func_0882FA10(float x, float y, void *motion, int id, int arg);
struct A31Motion {
    static void play(A31VActor *actor, int id) { func_0882FA10(0.0f, 0.0f, actor->motion, id, 0); }
};

extern "C" {

extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void func_0882FB90(float x, float y, void *motion, int id);
extern void func_08856D48(void *camera, A31VActor *actor, int mode, int arg);

void func_08A30A70(FinishEffect *self)
{
    A31Motion::play(self->actor, self->parts->level + 0x1FAA);
    float zero = 0.0f;
    {
        int id = 0x2134;
        func_0882FB90(zero, zero, self->actor->motion, id);
    }
    {
        int id = 1;
        func_0882FB90(zero, zero, self->actor->motion, id);
    }
    {
        A31ActorBits *bits = &self->target->bits;
        bits->b8 |= 0x80;
    }
    func_0882FA10(zero, zero, self->target->motion, 0x262, 0);
    {
        A31ActorBits *bits = &self->target->bits;
        bits->b8 &= ~0x80;
    }
    self->parts->stage = 4;
    func_08856D48(D_089D4DC8 + 0x69C0, self->actor, 4, 0);
    A31Spawn::at_hit(self->actor, 0x4E);
    for (int i = 0; i < self->parts->level; i++) {
        A31Spawn::at_hit(self->actor, i + 0x51);
    }
}

}
