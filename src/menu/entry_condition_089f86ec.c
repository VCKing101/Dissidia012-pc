#include "menu/agent29_types.h"

/* Screen0B6A0: whether an entry's condition holds: kind 8 needs at least
   `need` of func_089F32D4's count, kinds 6, 5 and 4 need a flag of the
   entry, other kinds always hold. */
extern int func_089F32D4(Screen0B6A0 *self, int arg1, int arg2);

int func_089F86EC(Screen0B6A0 *self, char kind, short need, Entry84 *entry)
{
    int ok = 0;

    switch (kind) {
    case 4:
        if (entry->flags & 4) {
            ok = 1;
        }
        break;
    case 5:
        if (entry->flags & 0x800) {
            ok = 1;
        }
        break;
    case 6:
        if (entry->flags & 8) {
            ok = 1;
        }
        break;
    case 8:
        if (func_089F32D4(self, 1, 0) >= need) {
            ok = 1;
        }
        break;
    default:
        ok = 1;
        break;
    }
    return ok;
}
