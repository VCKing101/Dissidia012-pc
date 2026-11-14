#include "battle/agent28_types.h"

/* Reloads the side flag's model for the owner's side. */
extern unsigned int func_08A22CA8(int owner);
extern void func_088DA060(void *model);
extern void *func_088DAA40(void *source, int name, int flags);
extern int D_08A06B68[2];

int func_08A0641C(SideFlag *flag)
{
    unsigned int side;

    if (!flag->open) {
        return -1;
    }
    side = func_08A22CA8(flag->owner);
    if (side >= 2) {
        return -2;
    }
    if (flag->model != 0) {
        func_088DA060(flag->model);
        flag->model = 0;
    }
    flag->model = func_088DAA40(flag->source, D_08A06B68[side], 0);
    if (flag->model == 0) {
        return -3;
    }
    return 0;
}
