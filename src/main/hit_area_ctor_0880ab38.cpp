#include "main/agent30_types.h"

/* Hit area constructor: clears both actor sets and the cooldown, builds the
   timers and appends the area to the global list. */
extern "C" {

extern HitAreaList D_089D4770;

void func_0880AB2C(HitAreaTimer *timer);
void func_08993560(void *array, int count, int size, void (*ctor)(HitAreaTimer *));

HitArea *func_0880AB38(HitArea *self, void *owner)
{
    self->owner = owner;
    int i;
    for (i = 0; i < 8; i++) {
        self->hit.words[i] = 0;
    }
    for (i = 0; i < 8; i++) {
        self->seen.words[i] = 0;
    }
    self->cooldown = 0.0f;
    func_08993560(self->timers, 4, sizeof(HitAreaTimer), func_0880AB2C);
    D_089D4770.append(self);
    return self;
}

}
