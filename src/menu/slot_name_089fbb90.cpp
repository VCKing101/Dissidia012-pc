#include "menu/agent29_types.h"

/* Screen0B6A0: makes an entry's name text; when `with_count` is set, as
   the message 0x60 with the name and the entry's count filled in. */
typedef struct PullSlot29 {
    char pad_00[0x6];
    short count;
    short name;
} PullSlot29;

extern "C" {
extern const char D_08A0AC54[];
PullSlot29 *func_089FCA28(Screen0B6A0 *self, unsigned short id);
void func_0887DF14(void *owner, char *out, int size, const char *text);
}

/* Inlined calls on the object in D_089D6998. */
struct Object9D6998 {
    static void Convert(char *out, int size, const char *text) {
        func_0887DF14(D_089D6998, out, size, text);
    }
};

/* Inlined lookup in the save's record table. */
struct Records29 {
    static Record8CB164 *Get(int id) { return func_088CB164(D_089D712C->field_F58, id); }
};

extern "C" void *func_089FBB90(Screen0B6A0 *self, unsigned short id, unsigned char with_count)
{
    PullSlot29 *slot = func_089FCA28(self, id);
    unsigned short name = slot->name;

    if (with_count) {
        char text[0x40];
        char *args[2];

        args[0] = func_0887B500(self->work->messages, 0, name);
        func_0893476C(text, (int)D_08A0AC54, slot->count);
        Object9D6998::Convert((char *)self->work + 0x24F8, 0x80, text);
        args[1] = (char *)self->work + 0x24F8;
        return func_089EF1E8(self, 0, 0x60, (int)args, 2);
    }
    return func_089EF1E8(self, 0, name, 0, 0);
}

/* Adds `amount` to the counts (+0x32B4, at most 10000) of the pull's
   records 1-4 that are set and not done, and copies each count into
   its record. */
