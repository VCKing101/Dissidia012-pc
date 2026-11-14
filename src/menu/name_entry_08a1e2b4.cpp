#include "menu/agent29_types.h"

/* MenuRoot: opens the name entry (+0x67B4, up to 0x9C bytes) for mode
   +0x6854: 0 edits the options' name, 1 the name of save slot +0x6858
   (empty for an unused slot), others start empty; sets the entry's
   layout (+0x6910) and limit, its title text and starts the keyboard. */
typedef struct NameOptions29 {
    char pad_00[0x36];
    char name[0x9C]; /* +0x36 */

    /* The option block at +0x652C of the root's work. */
    static NameOptions29 *Of(void *work) { return (NameOptions29 *)((char *)work + 0x652C); }
} NameOptions29;

typedef struct NameSlot29 {
    char pad_00[0x6];
    short used;      /* +0x06 */
    char pad_08[0x56];
    char name[0x16]; /* +0x5E */
    char pad_74[0xC4];

    static bool Empty(NameSlot29 *slot) { return slot->used == 0; }
} NameSlot29;

extern "C" {
extern const unsigned short D_08A23B2C[];
extern void *D_08B59230;
void func_08A1BBE8(MenuRoot *self, char *name);
void *func_08935524(void *dst, const void *src, int size);
void func_08B58DEC(void *keyboard);
}

extern "C" void func_08A1E2B4(MenuRoot *self)
{
    WORK_INT(self->work, 0x6928) = -1;
    WORK_INT(self->work, 0x692C) = -1;
    if (WORK_INT(self->work, 0x6854) == 0) {
        func_08935524(&WORK_BYTE(self->work, 0x67B4),
                      NameOptions29::Of(self->work)->name, 0x9C);
        func_08A1BBE8(self, (char *)&WORK_BYTE(self->work, 0x67B4));
        WORK_INT(self->work, 0x6910) = 0x4B;
    } else if (WORK_INT(self->work, 0x6854) == 1) {
        NameSlot29 *slots = (NameSlot29 *)((char *)D_089D712C + 0x39F38);
        NameSlot29 *slot = &slots[WORK_INT(self->work, 0x6858)];
        if (NameSlot29::Empty(slot)) {
            func_08935828(&WORK_BYTE(self->work, 0x67B4), 0, 0x16);
        } else {
            func_08935524(&WORK_BYTE(self->work, 0x67B4), slot->name, 0x16);
        }
        WORK_INT(self->work, 0x6910) = 0xA;
    } else {
        func_08935828(&WORK_BYTE(self->work, 0x67B4), 0, 0x9C);
        WORK_INT(self->work, 0x6910) = 0x4E;
    }
    WORK_INT(self->work, 0x6918) = 0x9C;
    WORK_INT(self->work, 0x68FC) = (int)func_0887B500(self->work->messages, 1, D_08A23B2C[WORK_INT(self->work, 0x6854)]);
    func_08B58DEC(D_08B59230);
}
