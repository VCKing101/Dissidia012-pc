#include "battle/oct13_activation_views.h"

int func_089E3358(void *object, BattleOct13Peer198 *peer)
{
    func_089E3300(object);
    if (!peer || !peer->field_198 || !peer->field_16C) return -1;
    {
        int i = 0;
        unsigned char *entry = (unsigned char *)object + 8;
        do {
            if (func_089E3A0C(entry, i, peer->field_198, peer->field_16C)) return -2;
            ++i;
            entry += 0xD8;
        } while (i < 2);
    }
    ((BattleOct13Prefix04 *)object)->field_04 = peer;
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}
