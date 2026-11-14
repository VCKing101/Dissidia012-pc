#include "battle/agent1_types.h"

/* Constructor, destructor, reset and release of the effect model. */
extern unsigned char D_089E68A0[];
extern void func_088DA060(void *);
extern void func_08992F04(void *);
void func_089E6138(BattleEffectModel *model);
void func_089E6184(BattleEffectModel *model);

BattleEffectModel *func_089E60A8(BattleEffectModel *model)
{
    model->vtable = D_089E68A0;
    func_089E6138(model);
    return model;
}

void func_089E60E0(BattleEffectModel *model, int flags)
{
    if (model) {
        model->vtable = D_089E68A0;
        func_089E6184(model);
        if (flags & 1) func_08992F04(model);
    }
}

void func_089E6138(BattleEffectModel *model)
{
    int i;

    model->field_00 = 0;
    model->field_04 = -1;
    model->field_08 = 0;
    model->field_0C = 0;
    model->field_10 = 0;
    model->field_14 = 0.0f;
    model->field_18 = 0.0f;
    model->field_1C = 0;
    model->field_20 = 0;
    for (i = 0; i < 2; i++) {
        model->field_24[i] = 0;
    }
}

void func_089E6184(BattleEffectModel *model)
{
    {
        int i;
        for (i = 0; i < 2; i++) {
            if (model->field_24[i]) {
                func_088DA060(model->field_24[i]);
                model->field_24[i] = 0;
            }
        }
    }
    if (model->field_20) {
        func_088DA060(model->field_20);
        model->field_20 = 0;
    }
    if (model->field_1C) {
        func_088DA060(model->field_1C);
        model->field_1C = 0;
    }
    func_089E6138(model);
}
