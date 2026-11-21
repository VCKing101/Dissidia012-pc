#include "menu/agent36_types.h"

/* Whether the actor id may be picked: 0x157, 0x53B and actors of kind 0x20
 * or 0x1C always; in mode 3 also every actor but kinds 7 and 0x16. */


struct State31760 {
    char pad_000[0x164];
    int mode;                 /* +0x164 */
};

struct Owner31760 {
    char pad_00[0x48];
    State31760 *state;        /* +0x48 */
};



extern "C" int func_08A31760(Owner31760 *self, int id)
{
    Actor36 *actor = func_088470B8(id);

    if (id == 0x157) {
        return 1;
    }
    if (actor != 0 && actor->kind == 0x20) {
        return 1;
    }
    if (actor != 0 && actor->kind == 0x1C) {
        return 1;
    }
    if (id == 0x53B) {
        return 1;
    }
    if (self->state->mode != 3) {
        return 0;
    }
    if (actor != 0 && actor->kind == 7) {
        return 0;
    }
    if (actor != 0 && actor->kind == 0x16) {
        return 0;
    }
    return 1;
}
