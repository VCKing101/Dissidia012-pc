#include "menu/agent36_types.h"

/* Name list screen: when row's entry changed, remakes its text and
 * sprite (or marks the row empty past the end). */

typedef struct Name3CD78 {
    unsigned short text;
} Name3CD78;

Name3CD78 *func_08A3C210(int id);

void func_08A3CD78(NameScreen36 *self, int row)
{
    int pair = row * 2;
    int index = row + self->work->top;

    if (self->work->shown[pair] != index) {
        self->work->shown[pair] = index;
        if (self->work->texts[pair] != 0) {
            func_0887BA5C(self->work->texts[pair]);
            self->work->texts[pair] = 0;
        }
        if (self->work->texts[pair + 1] != 0) {
            func_0887BA5C(self->work->texts[pair + 1]);
            self->work->texts[pair + 1] = 0;
        }
        if (index < self->work->count) {
            int text = func_08A3C210(self->work->ids[index])->text;
            self->work->texts[pair] = func_08A412A8(D_08A457EC, text, 0, 0);
            self->work->texts[pair + 1] = func_0887E534(self->work->texts[pair]);
            self->work->texts[pair + 1]->flags |= 0x800;
        } else {
            self->work->shown[pair] = 0xFFFF;
        }
    }
}
