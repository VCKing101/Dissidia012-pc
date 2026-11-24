// Detaches the pair's two effects from the owner's model node list, then frees both effects.
#include "battle/agent43_types.h"

struct Pair2 {
    int pad0[2];
    A43Model *model;
    A43Effect *effects[2];
};

struct EffectList {
    static void set(A43Effect **slot, A43Effect *value) { *slot = value; }
    static void detach(A43Model *model, A43Effect *effect)
    {
        if (effect != 0) {
            effect->flags &= ~0x8000;
            if (effect->prev == 0) {
                model->head = effect->next;
            } else {
                set(&effect->prev->next, effect->next);
            }
            if (effect->next == 0) {
                model->tail = effect->prev;
            } else {
                set(&effect->next->prev, effect->prev);
            }
            effect->prev = 0;
            effect->next = 0;
            model->count--;
        }
    }
};

extern "C" {
extern void func_0887BA5C(A43Effect *effect);

void func_08B05370(Pair2 *self)
{
    if (self->model != 0) {
        for (int i = 0; i < 2; i++) {
            if (self->effects[i] == 0) continue;
            EffectList::detach(self->model, self->effects[i]);
        }
    }
    for (int i = 0; i < 2; i++) {
        if (self->effects[i] != 0) {
            func_0887BA5C(self->effects[i]);
            self->effects[i] = 0;
        }
    }
}
}
