#include "menu/agent36_types.h"

/* Makes the texts of both groups of D_08A38FBC, each in its slot with its
 * sprite in the next slot (flag 0x800). */

typedef struct TextSlot36 {
    int slot;                 /* -1: end of the group */
    int kind;
} TextSlot36;

typedef struct TextObject36 {
    char pad_00[0x10];
    unsigned int flags;       /* +0x10 */
} TextObject36;

typedef struct Work35B08 {
    char pad_00[0xC];
    TextObject36 *texts[1];   /* +0x0C */
} Work35B08;

typedef struct Screen35B08 {
    char pad_000[0xF64];
    Work35B08 *work;          /* +0xF64 */
} Screen35B08;

extern TextSlot36 D_08A38FBC[2][3];


void func_08A35B08(Screen35B08 *self)
{
    int i;
    int j;

    for (i = 0; i < 2; i++) {
        for (j = 0; D_08A38FBC[i][j].slot != -1; j++) {
            int kind = D_08A38FBC[i][j].kind;
            int slot = D_08A38FBC[i][j].slot;
            self->work->texts[slot] = (TextObject36 *)func_08A412A8(D_08A457EC, kind, 0, 0);
            self->work->texts[slot + 1] = (TextObject36 *)func_0887E534(self->work->texts[slot]);
            self->work->texts[slot + 1]->flags |= 0x800;
        }
    }
}
