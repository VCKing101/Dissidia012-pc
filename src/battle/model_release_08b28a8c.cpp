// Detaches the holder's effect from its model's node list and frees it, then frees the model; the model must be a kind-8 object with flag 0x20.
#include "battle/agent43_types.h"

struct A43Model8 {
    unsigned char pad_00[0xC];
    short kind;
    unsigned char pad_0e[0x12];
    unsigned short flags;
};

struct Holder8 {
    int pad0;
    A43Model8 *model;
    A43Effect *effect;
};

struct EffectList {
    static void set(A43Effect **slot, A43Effect *value) { *slot = value; }
    static bool Has(unsigned v, unsigned m) { return (v & m) != 0; }
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
extern void func_088DA060(void *model);

int func_08B28A8C(Holder8 *self)
{
    if (self->model != 0 && self->model->kind == 8) {
        int one = 1;
        if (EffectList::Has(self->model->flags, 0x20) == one) {
            if (self->effect != 0) {
                EffectList::detach((A43Model *)self->model, self->effect);
            }
            if (self->effect != 0) {
                func_0887BA5C(self->effect);
                self->effect = 0;
            }
            if (self->model != 0) {
                func_088DA060(self->model);
                self->model = 0;
            }
        }
    }
    return 0;
}
}
