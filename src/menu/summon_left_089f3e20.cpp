#include "menu/agent29_types.h"

/* Screen0B6A0: rebuilds the "pulls left" text (texts[0x15]): the record's
   count less the pulls made (+0x2746), plus the two bonus slots and every
   used entry. */
extern "C" {
extern const char D_08A0AC54[];
extern const unsigned short D_08A09B2A[];
int func_089F0920(Screen0B6A0 *self, int slot);
void func_0887DF14(void *owner, char *out, int size, const char *text);
}

/* Inlined call on the object in D_089D6998. */
struct Object9D6998 {
    static void Convert(char *out, int size, const char *text) {
        func_0887DF14(D_089D6998, out, size, text);
    }
};

extern "C" void func_089F3E20(Screen0B6A0 *self)
{
    char text[0x40];
    char *args[1];
    int left;

    if (self->work->texts[0x15] != 0) {
        func_0887BA5C(self->work->texts[0x15]);
        self->work->texts[0x15] = 0;
    }
    left = WORK_SHORT(self->work->field_68, 0xA) - WORK_U16(self->work, 0x2746);
    if (func_089F0920(self, 1) != 0) {
        left++;
    }
    if (func_089F0920(self, 2) != 0) {
        left++;
    }
    for (int i = 0; i < 9; i++) {
        if (self->work->entries[i].kind != 0) {
            left++;
        }
    }
    func_0893476C(text, (int)D_08A0AC54, left);
    Object9D6998::Convert((char *)self->work + 0x1D78, 0x80, text);
    args[0] = (char *)self->work + 0x1D78;
    self->work->texts[0x15] = func_089EF1E8(self, 0, 0xCD, (int)args, 1);
    func_088D428C(self->work->handles, self->work->texts, D_08A09B2A, 1);
}
