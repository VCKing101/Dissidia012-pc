#include "menu/agent29_types.h"

/* Screen0B6A0: the collection rate in percent: of the free rewards of the
   data block (+0x34) and their second forms (+0x0B), how many the save's
   collection (+0x1E208) holds. */
typedef struct Reward29 {
    short id;
    char pad_02[0x8];
    signed char enabled; /* +0x0A */
    signed char has_alt; /* +0x0B */
    char pad_0C[0x8];
    short cost;          /* +0x14 */
    char pad_16[0x1A];
} Reward29;

extern int func_088F3DA8(void *collection, unsigned short id);
extern int func_088F3D48(void *collection, unsigned short id);

int func_089FCD38(Screen0B6A0 *self)
{
    int i;
    int total = 0;
    int owned = 0;
    Reward29 *reward;

    for (i = 0; i < self->work->header->field_22; i++) {
        reward = (Reward29 *)self->work->field_34 + i;
        if (reward->cost == 0) {
            total++;
            if (func_088F3DA8((char *)D_089D712C + 0x1E208, reward->id) != 0) {
                owned++;
            }
            if (reward->has_alt != 0) {
                total++;
                if (func_088F3D48((char *)D_089D712C + 0x1E208, reward->id) != 0) {
                    owned++;
                }
            }
        }
    }
    return owned * 100 / total;
}
