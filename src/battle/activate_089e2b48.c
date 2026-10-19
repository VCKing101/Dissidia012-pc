#include "battle/runtime_helpers.h"

/* Connected activation/state helpers; complete class ownership is unknown. */
extern void func_089E2B2C(void *);

int func_089E2B48(unsigned char *object, unsigned int index, void *input)
{
    func_089E2B2C(object);
    if (index >= 2 || !input) return -1;
    ((BattleBinding2B48 *)object)->field_10 = index;
    ((BattleBinding2B48 *)object)->field_14 = input;
    object[0] = 1;
    return 0;
}
