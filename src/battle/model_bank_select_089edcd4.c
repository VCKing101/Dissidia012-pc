#include "battle/agent28_types.h"

/* Swaps the bank's selection models for the given selection kind and the
 * owner's side colour. */
extern unsigned int func_089E95E0(int owner);
extern void func_088DA060(void *model);
extern void *func_088DAA40(void *source, int name, int flags);
extern void func_089EDE48(ModelBank *bank);

int func_089EDCD4(ModelBank *bank, unsigned int kind)
{
    unsigned int side;

    if (!bank->open) {
        return -1;
    }
    if (kind >= 4) {
        return -2;
    }
    side = func_089E95E0(bank->owner);
    if (side >= 2) {
        return -3;
    }
    if (bank->frame != 0) {
        func_088DA060(bank->frame);
        bank->frame = 0;
    }
    if (bank->icon != 0) {
        func_088DA060(bank->icon);
        bank->icon = 0;
    }
    {
        int frames[2][4] = { { 8, 8, 5, 2 }, { 7, 7, 6, 3 } };
        int icons[4] = { 0, 1, 4, 4 };
        bank->icon = func_088DAA40(bank->source, icons[kind], 0);
        bank->frame = func_088DAA40(bank->source, frames[side][kind], 0);
    }
    if (bank->frame == 0) {
        if (bank->icon != 0) {
            func_088DA060(bank->icon);
            bank->icon = 0;
        }
        return -4;
    }
    func_089EDE48(bank);
    return 0;
}
