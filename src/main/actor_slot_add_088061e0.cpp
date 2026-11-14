#include "main/agent30_types.h"

/* Adds an effect slot to an actor: a timed slot when a duration is given,
   otherwise a fixed one. */
struct Actor088061E0 {
    char pad_000[0x584];
    ActorEffectSlots effects;
};

extern "C" {

void func_088061E0(Actor088061E0 *self, int kind, int level, int frames)
{
    bool timed = frames > 0;
    if (timed) {
        for (int i = 0; i < 16; i++) {
            ActorEffectSlot *slot = &self->effects.timed[i];
            if (!slot->active) {
                ActorEffectSlot::set(slot, kind, level, frames);
                return;
            }
        }
    } else {
        for (int i = 0; i < 16; i++) {
            ActorEffectSlot *slot = &self->effects.fixed[i];
            if (!slot->active) {
                ActorEffectSlot::set(slot, kind, level, 0);
                return;
            }
        }
    }
}

}
