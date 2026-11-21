#include "menu/agent36_types.h"

/* Options page: unless busy, takes the options copy back when the save
 * check passes (and marks id 0x48 known), then state 2. */

struct Work4C928 {
    char pad_00[0x4];
    int state;                /* +0x04 */
    char pad_08[0x8];
    RootOptions36 *source;    /* +0x10 */
    RootOptions36 options;    /* +0x14 */
};

struct Screen4C928 {
    char pad_00[0x5C];
    Work4C928 *work;          /* +0x5C */
};

extern "C" {
extern unsigned char *D_089D7280;
void func_08A4D824(Screen4C928 *self, int arg);
int func_088AF29C(void *object);
void func_08935524(void *dst, const void *src, int size);   /* memcpy */
}

extern "C" void func_08A4C928(Screen4C928 *self)
{
    if (*D_089D7280 == 0) {
        func_08A4D824(self, 0);
        if (func_088AF29C(D_089D7260) != 0) {
            func_08935524(&self->work->options, self->work->source, sizeof(RootOptions36));
            Id36::SetKnown(0x48);
            D_09B6DCE8.field_13 = 0;
        }
        func_088AF044(D_089D7260);
        self->work->state = 2;
    }
}
