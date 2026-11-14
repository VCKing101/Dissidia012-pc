#include "battle/agent28_types.h"

/* Releases the eleven models of a model bank and resets it. */
extern void func_088DA060(void *model);
extern void func_089ED928(ModelBank *bank);

void func_089ED974(ModelBank *bank)
{
    if (bank->icon != 0) {
        func_088DA060(bank->icon);
        bank->icon = 0;
    }
    if (bank->frame != 0) {
        func_088DA060(bank->frame);
        bank->frame = 0;
    }
    if (bank->models[8] != 0) {
        func_088DA060(bank->models[8]);
        bank->models[8] = 0;
    }
    if (bank->models[7] != 0) {
        func_088DA060(bank->models[7]);
        bank->models[7] = 0;
    }
    if (bank->models[6] != 0) {
        func_088DA060(bank->models[6]);
        bank->models[6] = 0;
    }
    if (bank->models[5] != 0) {
        func_088DA060(bank->models[5]);
        bank->models[5] = 0;
    }
    if (bank->models[4] != 0) {
        func_088DA060(bank->models[4]);
        bank->models[4] = 0;
    }
    if (bank->models[3] != 0) {
        func_088DA060(bank->models[3]);
        bank->models[3] = 0;
    }
    if (bank->models[2] != 0) {
        func_088DA060(bank->models[2]);
        bank->models[2] = 0;
    }
    if (bank->models[1] != 0) {
        func_088DA060(bank->models[1]);
        bank->models[1] = 0;
    }
    if (bank->models[0] != 0) {
        func_088DA060(bank->models[0]);
        bank->models[0] = 0;
    }
    func_089ED928(bank);
}
