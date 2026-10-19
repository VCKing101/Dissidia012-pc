#include "battle/oct13_activation_views.h"

int func_089E7258(void *object, BattleOct13Peer1CC *peer, void *shared)
{
    func_089E7200(object);
    if (!peer || !peer->field_1CC || !shared) return -1;
    {
        int i = 0;
        unsigned char *entry = (unsigned char *)object + 4;
        do {
            if (func_089E74A8(entry, i, peer->field_1CC, shared)) return -2;
            ++i;
            entry += 0x20;
        } while (i < 2);
    }
    ((BattleOct13Prefix04 *)object)->field_00 = 1;
    return 0;
}
