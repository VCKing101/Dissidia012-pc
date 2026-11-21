#include "battle/agent31_types.h"

/* Movement state at A31Actor.move, as the fall handler sees it. */
typedef struct FallState {
    unsigned char airborne;
    unsigned char heavy;
    unsigned char pad_02[0xA];
    float gravity;
} FallState;

extern float D_089D48E8;
extern float D_089D48EC;

extern int func_088374B0(A31Actor *actor);
extern void func_0882FA10(void *motion, int id, float x, float y, int arg);

void func_08A2D8D8(int unused, A31Actor *actor)
{
    FallState *state = (FallState *)actor->move;

    state->airborne = 0;
    state->gravity = D_089D48E8;
    if (func_088374B0(actor) != 0) {
        func_0882FA10(actor->motion, 0x4E23, 0.0f, 0.0f, 0);
        state->gravity = D_089D48EC;
        state->heavy = 1;
    } else {
        func_0882FA10(actor->motion, 0x4E20, 0.0f, 0.0f, 0);
        state->heavy = 0;
    }
    actor->flags |= 0x200000;
}
