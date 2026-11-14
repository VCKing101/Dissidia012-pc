#include "battle/agent28_types.h"

/* Exit state: once the model is hidden (or gone), frees the four effects
 * and the model and moves to state 4. */
struct BoundModel {
    unsigned char pad_00[0x20];
    unsigned short flags;

    static bool hidden(BoundModel *model) { return (model->flags & 0x20) != 0; }
};

struct EffectList {
    static void set(ModelEffect **slot, ModelEffect *value) { *slot = value; }
    static void detach(EffectModel *model, ModelEffect *effect)
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

extern void func_0887BA5C(ModelEffect *effect);
extern void func_088DA060(EffectModel *model);

int func_089EC5F8(CardRecord *record)
{
    if (record->model == 0 || BoundModel::hidden((BoundModel *)record->model) == 1) {
        for (int i = 0; i < 4; i++) {
            if (record->effects[i] != 0) {
                if (record->model != 0) {
                    EffectList::detach(record->model, record->effects[i]);
                }
                if (record->effects[i] != 0) {
                    func_0887BA5C(record->effects[i]);
                    record->effects[i] = 0;
                }
            }
        }
        if (record->model != 0) {
            func_088DA060(record->model);
            record->model = 0;
        }
        record->state = 4;
        record->landed = 0;
    }
    return 0;
}

}
