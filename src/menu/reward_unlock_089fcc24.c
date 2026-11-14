#include "menu/agent29_types.h"

/* Screen0B6A0: unlocks (func_088F3DD8 on the save's collection at
   +0x1E208) every reward of the data block (+0x34) that is enabled, free,
   not yet allowed and needs at most `level`; gives 1 when it unlocked
   any. */
typedef struct Reward29 {
    short id;
    char pad_02[0x4];
    short level;         /* +0x06 */
    char pad_08[0x2];
    signed char enabled; /* +0x0A */
    char pad_0B[0x9];
    short cost;          /* +0x14 */
    char pad_16[0x1A];
} Reward29;

extern int func_088F3D18(void *collection, unsigned short id);
extern void func_088F3DD8(void *collection, unsigned short id);

int func_089FCC24(Screen0B6A0 *self, int level)
{
    int unlocked = 0;
    int i;
    Reward29 *reward;

    for (i = 0; i < self->work->header->field_22; i++) {
        reward = (Reward29 *)self->work->field_34 + i;
        if (reward->enabled != 0 && reward->cost == 0
            && func_088F3D18((char *)D_089D712C + 0x1E208, reward->id) == 0
            && ((Reward29 *)self->work->field_34)[i].level <= level) {
            func_088F3DD8((char *)D_089D712C + 0x1E208, reward->id);
            unlocked = 1;
        }
    }
    return unlocked;
}
