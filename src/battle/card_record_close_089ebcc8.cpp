#include "battle/agent28_types.h"

/* Closes a card record: detaches and frees its four effects, frees its
 * model and resets it. */
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
extern void func_089EBC54(CardRecord *record);

void func_089EBCC8(CardRecord *record)
{
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
    func_089EBC54(record);
}

}
