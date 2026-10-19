#include "battle/runtime_helpers.h"

/* Observed state/update helpers; original gameplay ownership is unresolved. */
extern void func_089E381C(void *);

int func_089E3768(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleFlagSlots *)object)->field_1B8[index] = 0;
    func_089E381C(object);
    return 0;
}

int func_089E37C0(unsigned char *object, unsigned int index)
{
    if (!object[0]) return -1;
    if (index >= 3) return -2;
    ((BattleFlagSlots *)object)->field_1B8[index] = 1;
    func_089E381C(object);
    return 0;
}
