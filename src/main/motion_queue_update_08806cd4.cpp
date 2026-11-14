#include "main/agent30_types.h"

/* When the current motion has run out, starts the next queued one on the
   actor; then counts the timer down by the actor's time step. */
extern "C" {

void func_088065B4(void *actor, void *motion, int kind, float time);
float func_08838300(void *actor);

void func_08806CD4(MotionQueue *self, void *actor)
{
    if (self->timer <= 0.0f && 0 < self->count) {
        unsigned char code = self->codes[self->head];
        self->timer = (code & 0x1F) * 2.0f;
        self->slot = code >> 5;
        void *motion = self->motions[self->slot];
        if (motion != 0) {
            func_088065B4(actor, motion, 1, self->timer);
        }
        self->head++;
        if (self->head >= 64) {
            self->head = 0;
        }
        self->count--;
    }
    self->timer -= func_08838300(actor);
}

}
