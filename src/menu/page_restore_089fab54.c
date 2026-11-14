#include "menu/agent29_types.h"

/* Screen0B6A0: restores the page state (+0x2724, 0x9AC bytes) saved in
   D_09B6DA40 (made first when there is none), takes the first record id
   as the current one (+0x328C) and redraws. */
typedef struct PageState29 {
    int words[0x9AC / 4];
} PageState29;

typedef struct PullState29 {
    int field_00;
    int field_04;
    int mode;
    int music;
    PageState29 *saved;  /* +0x10 */
} PullState29;

extern PullState29 D_09B6DA40;
extern void func_08A04E84(Screen0B6A0 *self);
extern void func_089FA91C(Screen0B6A0 *self);
extern void func_089FAA2C(Screen0B6A0 *self);

void func_089FAB54(Screen0B6A0 *self)
{
    PullState29 *pull;

    if (D_09B6DA40.field_00 == 0) {
        func_08A04E84(self);
    }
    pull = &D_09B6DA40;
    *(PageState29 *)&self->work->flags = *pull->saved;
    WORK_INT(self->work, 0x328C) = self->work->field_2E84[0];
    func_089FA91C(self);
    func_089FAA2C(self);
}
