#include "menu/agent29_types.h"

/* Screen0B6A0: rebuilds the pull-count text (count +0x32C8) and its
   shadow, resets the text animation and starts it. */
struct TextSlot29 {
    unsigned short field_00;
    unsigned short text;    /* index into the work block's texts */
};

extern "C" {
extern const TextSlot29 D_08A0A672;
extern const char D_08A0AC54[];
void func_0887DF14(void *owner, char *out, int size, const char *text);
void *func_0887D868(void *owner, int arg1, const char *text, int arg3, int arg4, int arg5, int arg6,
                    int arg7);
void func_089FF848(Screen0B6A0 *self);
}

/* Inlined calls on the object in D_089D6998. */
struct Object9D6998 {
    static void Convert(char *out, int size, const char *text) {
        func_0887DF14(D_089D6998, out, size, text);
    }
    static void *Make(const char *text) {
        return func_0887D868(D_089D6998, 0, text, -1, 0, 0, 0, 0);
    }
};

extern "C" void func_089F277C(Screen0B6A0 *self)
{
    char text[0x40];
    const TextSlot29 *slot = &D_08A0A672;

    if (self->work->texts[slot->text] != 0) {
        func_0887BA5C(self->work->texts[slot->text]);
        self->work->texts[slot->text] = 0;
    }
    if (self->work->texts[0xB] != 0) {
        func_0887BA5C(self->work->texts[0xB]);
        self->work->texts[0xB] = 0;
    }
    func_0893476C(text, (int)D_08A0AC54, WORK_INT(self->work, 0x32C8));
    Object9D6998::Convert((char *)self->work + 0xE78, 0x80, text);
    self->work->texts[slot->text] = Object9D6998::Make((char *)self->work + 0xE78);
    self->work->texts[0xB] = func_0887E534(self->work->texts[slot->text]);
    WORK_INT(self->work, 0x31EC) = 0;
    WORK_SHORT(self->work, 0x31F0) = 1;
    WORK_SHORT(self->work, 0x31F2) = 0;
    {
        int none = -1;
        WORK_INT(self->work, 0x31F8) = none;
    }
    func_089FF848(self);
}
