#include "menu/agent36_types.h"

/* Counts the unlock items of group whose prerequisite id is set and known. */
extern "C" int func_08A29840(int group)
{
    int total = 0;
    if (D_08A29BF0 != 0) {
        for (int i = 0; i < D_08A29BF0->counts[group]; i++) {
            Unlock36 *items = D_09B6D9F8[group];
            Unlock36 *item = &items[i];
            if (!Id36::IsNone(item->needs_id) && Id36::IsKnown(item->needs_id)) {
                total++;
            }
        }
    }
    return total;
}
