#include "battle/oct13_activation_views.h"

int func_089ED064(void *object)
{
    func_089ED01C((unsigned char *)object);
    ((BattleOct13Prefix04 *)object)->field_04 = func_088D09D8(D_089D7830, 1, 0x1F);
    if (!((BattleOct13Prefix04 *)object)->field_04) return -1;
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}
