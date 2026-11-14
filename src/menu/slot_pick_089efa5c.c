#include "menu/agent29_types.h"

/* Screen0B6A0: the slot id for `kind`: kinds 9 and 0x12 are looked up
   directly; any other kind only when there is a kind-9 slot or no
   kind-0x12 one, and not when the kind-9 slot makes it a repeat. */
extern int func_089EF99C(Screen0B6A0 *self, unsigned char kind);
extern int func_089EF8F4(Screen0B6A0 *self, int id);

int func_089EFA5C(Screen0B6A0 *self, unsigned char kind)
{
    int found = 0;
    int id = func_089EF99C(self, 9);

    if (kind == 9) {
        return id;
    }
    if (id != -1) {
        found = 1;
    }
    id = func_089EF99C(self, 0x12);
    if (kind == 0x12) {
        return id;
    }
    if (!found && id != -1) {
        return -1;
    }
    id = func_089EF99C(self, kind);
    if (func_089EF8F4(self, id) == 1 && found) {
        id = -1;
    }
    return id;
}
