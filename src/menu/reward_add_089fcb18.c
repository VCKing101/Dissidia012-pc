#include "menu/agent29_types.h"

/* Screen0B6A0: adds to the save's collection (+0x1E208) every reward of
   the data block (+0x34, header count +0x22) that is enabled (+0x0A),
   free (+0x14 clear), not yet owned and allowed; gives 1 when it added
   any. */
typedef struct Reward29 {
    short id;
    char pad_02[0x8];
    signed char enabled; /* +0x0A */
    char pad_0B[0x9];
    short cost;          /* +0x14 */
    char pad_16[0x1A];
} Reward29;

extern int func_088F3D78(void *collection, unsigned short id);
extern int func_088F3D18(void *collection, unsigned short id);
extern void func_088F3E38(void *collection, unsigned short id);

int func_089FCB18(Screen0B6A0 *self)
{
    int added = 0;
    int i;
    Reward29 *reward;

    for (i = 0; i < self->work->header->field_22; i++) {
        reward = (Reward29 *)self->work->field_34 + i;
        if (reward->enabled != 0 && reward->cost == 0
            && func_088F3D78((char *)D_089D712C + 0x1E208, reward->id) == 0
            && func_088F3D18((char *)D_089D712C + 0x1E208, reward->id) != 0) {
            func_088F3E38((char *)D_089D712C + 0x1E208, reward->id);
            added = 1;
        }
    }
    return added;
}
