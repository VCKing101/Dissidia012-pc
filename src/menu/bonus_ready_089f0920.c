#include "menu/agent29_types.h"

/* Screen0B6A0: whether bonus slot `slot` (1 or 2) can be used: not when
   its page flag (0x20000 / 0x40000) is set, and only when the save has
   the slot and its entry is the current one (+0x272E). */
typedef struct BonusSlot29 {
    char pad_00[0xE];
    unsigned short entry;
} BonusSlot29;

extern int func_088F2D8C(void *table, int slot);
extern BonusSlot29 *func_088F2DC0(void *table, int slot);

int func_089F0920(Screen0B6A0 *self, int slot)
{
    BonusSlot29 *bonus;

    if (slot == 1 && (self->work->flags & 0x20000) != 0) {
        return 0;
    } else if (slot == 2 && (self->work->flags & 0x40000) != 0) {
        return 0;
    } else if (func_088F2D8C((char *)D_089D712C + 0x1E208, slot) == 0) {
        return 0;
    } else {
        bonus = func_088F2DC0((char *)D_089D712C + 0x1E208, slot);
        return WORK_U16(self->work, 0x272E) == bonus->entry;
    }
}
