#include "battle/agent31_types.h"

typedef struct A31Vec3 {
    float x, y, z;
} A31Vec3;

/* Movement state at A31Actor.move, as the landing handler sees it. */
typedef struct LandState {
    unsigned char airborne;
    unsigned char pad_01[3];
    float rise;
    float fall;
    unsigned char pad_0c[0x14];
    A31Vec3 start;
    unsigned char pad_2c[4];
    A31Vec3 last;
    unsigned char pad_3c[4];
    int count_40;
    int count_44;
    int count_48;
} LandState;

extern void func_0882FA10(float x, float y, void *motion, int id, int arg);
extern void func_089A3868(A31Vec4 *out, void *matrix);

void func_08A2C828(int unused, A31Actor *actor)
{
    float zero = 0.0f;
    LandState *state = (LandState *)actor->move;
    A31Vec4 at;

    func_0882FA10(zero, zero, actor->motion, 0x4E2A, 0);
    state->airborne = 0;
    state->fall = zero;
    state->rise = zero;
    state->count_48 = 0;
    state->count_44 = 0;
    state->count_40 = 0;
    func_089A3868(&at, actor->pad_000 + 0x90);
    state->last.x = at.x;
    state->last.y = at.y;
    state->last.z = at.z;
    state->start.x = at.x;
    state->start.y = at.y;
    state->start.z = at.z;
    actor->flags |= 0x200000;
}
