#include "main/agent30_types.h"

/* Per-frame update of an actor's effect slots: every active slot is
   re-applied, and the timed slots count down by the actor's time step and
   clear themselves when they run out. */
struct Actor08804BA0 {
    char pad_000[0x584];
    ActorEffectSlots effects;
};

extern "C" {

void func_08831CEC(Actor08804BA0 *actor, int kind, int level);
float func_08838300(Actor08804BA0 *actor);

void func_08804BA0(Actor08804BA0 *self)
{
    for (int i = 0; i < 16; i++) {
        ActorEffectSlot *slot = &self->effects.fixed[i];
        if (slot->active) {
            func_08831CEC(self, slot->kind, slot->level);
        }
    }
    for (int i = 0; i < 16; i++) {
        ActorEffectSlot *slot = &self->effects.timed[i];
        if (slot->active) {
            func_08831CEC(self, slot->kind, slot->level);
            if (!slot->held) {
                slot->time -= func_08838300(self);
                if (slot->time <= 0.0f) {
                    slot->active = 0;
                    slot->held = 0;
                    slot->kind = 0;
                    slot->level = 0;
                    slot->time = 0.0f;
                }
            }
        }
    }
}

}
