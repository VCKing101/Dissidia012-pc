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

/* Adds `amount` to the counts (+0x32B4, at most 10000) of the pull's
   records 1-4 that are set and not done, and copies each count into
   its record. */
extern "C" void func_089FC7EC(Screen0B6A0 *self, int amount)
{
    int i;
    Record8CB164 *record;

    for (i = 1; i < 5; i++) {
        if (self->work->field_2E84[i] != 0 && ((&WORK_INT(self->work, 0x2E98))[i] & 1) == 0) {
            (&WORK_INT(self->work, 0x32B4))[i] += amount;
            if ((&WORK_INT(self->work, 0x32B4))[i] > 10000) {
                (&WORK_INT(self->work, 0x32B4))[i] = 10000;
            }
            record = Records29::Get(self->work->field_2E84[i]);
            record->field_06 = (&WORK_INT(self->work, 0x32B4))[i];
        }
    }
}
