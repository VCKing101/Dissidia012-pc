#include "menu/agent29_types.h"

/* Screen0B6A0: picks the pull music of the current entry (0x3C when it
   has none); returns 1 when that changes the music in D_09B6DA40. */
typedef struct PullState29 {
    int field_00;
    int field_04;
    int mode;
    int music;       /* +0x0C */
    void *sound;
} PullState29;

typedef struct PullEntry29 {
    char pad_00[0xE];
    short music;
} PullEntry29;

extern PullState29 D_09B6DA40;
extern PullEntry29 *func_089FCA28(Screen0B6A0 *self, unsigned short entry);

int func_089FE4CC(Screen0B6A0 *self)
{
    int changed = 0;
    int music = func_089FCA28(self, WORK_U16(self->work, 0x272E))->music;
    PullState29 *pull = &D_09B6DA40;

    if (music < 0) {
        music = 0x3C;
    }
    if (pull->music != music) {
        pull->music = music;
        changed = 1;
    }
    return changed;
}
