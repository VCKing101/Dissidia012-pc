#include "menu/agent29_types.h"

/* Screen0B6A0: rebuilds the "pulls / total" text (texts[0x13], from
   +0x2728 and +0x27A4), the entry's name text (texts[0x14]) and the
   pulls-left text. */
extern "C" {
extern const char D_08A0AC54[];
void func_0887DF14(void *owner, char *out, int size, const char *text);
void *func_089FBB90(Screen0B6A0 *self, unsigned short entry, int arg);
void func_089F3E20(Screen0B6A0 *self);
}

/* Inlined call on the object in D_089D6998. */
struct Object9D6998 {
    static void Convert(char *out, int size, const char *text) {
        func_0887DF14(D_089D6998, out, size, text);
    }
};

extern "C" void func_089F3CC8(Screen0B6A0 *self)
{
    char text[0x40];
    char *args[2];

    if (self->work->texts[0x13] != 0) {
        func_0887BA5C(self->work->texts[0x13]);
        self->work->texts[0x13] = 0;
    }
    func_0893476C(text, (int)D_08A0AC54, WORK_SHORT(self->work, 0x2728));
    Object9D6998::Convert((char *)self->work + 0x1F78, 0x80, text);
    args[0] = (char *)self->work + 0x1F78;
    func_0893476C(text, (int)D_08A0AC54, WORK_INT(self->work, 0x27A4));
    Object9D6998::Convert((char *)self->work + 0x1FF8, 0x80, text);
    args[1] = (char *)self->work + 0x1FF8;
    self->work->texts[0x13] = func_089EF1E8(self, 0, 0xD, (int)args, 2);
    if (self->work->texts[0x14] != 0) {
        func_0887BA5C(self->work->texts[0x14]);
        self->work->texts[0x14] = 0;
    }
    self->work->texts[0x14] = func_089FBB90(self, WORK_U16(self->work, 0x272E), 1);
    func_089F3E20(self);
}
