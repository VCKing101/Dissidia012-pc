#include "battle/agent31_types.h"

extern void func_088357B8(A31Actor *actor, int mode);
extern void func_08835BA4(A31Actor *actor, A31Vec4 *pos);
extern int func_0883739C(A31Actor *actor);
extern void func_088118F8(A31Timer *timer);
extern void func_08834C10(A31Actor *actor, int state, int arg);
extern void func_0882FA10(float x, float y, void *motion, int id, int arg);

void func_08A2AE70(int unused, A31Actor *actor, A31Vec4 *pos, unsigned char hold)
{
    A31Actor *self = actor;
    A31Vec4 at;

    func_088357B8(actor, 1);
    at = *pos;
    func_08835BA4(actor, &at);
    actor->state_flags |= 2;
    if (func_0883739C(actor) != 0 && (unsigned char)(self->timer.active != 0)) {
        func_088118F8(&self->timer);
    }
    if (hold) {
        actor->flags |= 0x1000;
        func_08834C10(actor, 0x12, 0);
    } else {
        func_08834C10(actor, 0, 0);
        func_0882FA10(0.0f, 0.0f, actor->motion, 1, 0);
    }
}
