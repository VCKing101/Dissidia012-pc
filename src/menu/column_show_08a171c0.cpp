#include "menu/agent29_types.h"

/* Screen181E0: shows (show == 1) or hides the four models of column
   `column` (0x53, 0x58, 0x5D and 0x62 + column) and, when shown with a
   value, loads its two digit pictures (0x2050 and 0x2030 + value) into
   layout entries 0x3A and 0x3F + column. */
extern "C" {
extern void *D_089D7830;
void func_088D0984(void *loader, void *entry, int kind, unsigned short picture);
}

/* Inlined calls on the loader in D_089D7830. */
struct Loader9D7830 {
    static void Picture(void *entry, char kind, unsigned short picture) {
        func_088D0984(D_089D7830, entry, kind, picture);
    }
};

extern "C" void func_08A171C0(Screen181E0 *self, unsigned char show, int column, int value)
{
    if (show == 1) {
        func_088D9CD8(self->work->handles[0x53 + column]);
        func_088D9CD8(self->work->handles[0x58 + column]);
        func_088D9CD8(self->work->handles[0x5D + column]);
        func_088D9CD8(self->work->handles[0x62 + column]);
        if (value > 0) {
            Loader9D7830::Picture(self->work->layout.entries[0x3A + column], 9, value + 0x2050);
            Loader9D7830::Picture(self->work->layout.entries[0x3F + column], 9, value + 0x2030);
        }
    } else {
        func_088D9CFC(self->work->handles[0x53 + column]);
        func_088D9CFC(self->work->handles[0x58 + column]);
        func_088D9CFC(self->work->handles[0x5D + column]);
        func_088D9CFC(self->work->handles[0x62 + column]);
    }
}
