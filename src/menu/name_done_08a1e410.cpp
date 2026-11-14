#include "menu/agent29_types.h"

/* MenuRoot: closes the name entry (+0x67B4) for mode +0x6854: mode 0
   either rejects the name (func_08A1E588; then sets the list mode once
   and goes to step 4) or stores it as the options' name; mode 1 stores a
   non-empty name into save slot +0x6858 (stamped with the date and the
   profile's id), an empty one goes to step 1. */
typedef struct NameOptions29 {
    char pad_00[0x36];
    char name[0x9C]; /* +0x36 */

    /* The option block at +0x652C of the root's work. */
    static NameOptions29 *Of(void *work) { return (NameOptions29 *)((char *)work + 0x652C); }
} NameOptions29;

typedef struct NameSlot29 {
    char data[0x138];
} NameSlot29;

typedef struct NameProfile29 {
    char pad_00[0xC];
    int id; /* +0x0C */
} NameProfile29;

extern "C" {
int func_08A1E588(MenuRoot *self, char *name);
void func_08A1C028(MenuRoot *self, unsigned char on);
void func_08A1BB70(MenuRoot *self, char *name);
void *func_08935524(void *dst, const void *src, int size);
void func_0887E81C(char *dst, const char *src, int size);
void func_089C5D98(char *date);
void func_088C7900(NameSlot29 *slot, char *owner, char *name, int id);
}

extern "C" void func_08A1E410(MenuRoot *self)
{
    char date[0x10];

    if (WORK_INT(self->work, 0x6854) == 0) {
        if (func_08A1E588(self, (char *)&WORK_BYTE(self->work, 0x67B4)) != 0) {
            if (WORK_INT(self->work, 0x6928) == 0) {
                if (WORK_INT(self->work, 0x2BB8) & 1) {
                    WORK_INT(self->work, 0x6928) = 1;
                    func_08A1C028(self, 0);
                } else {
                    WORK_INT(self->work, 0x6928) = 0;
                    func_08A1C028(self, 1);
                }
            }
            WORK_INT(self->work, 0x685C) = 4;
        } else {
            char *name = NameOptions29::Of(self->work)->name;
            func_08935524(name, &WORK_BYTE(self->work, 0x67B4), 0x9C);
            func_08A1BB70(self, name);
        }
    } else if (WORK_INT(self->work, 0x6854) == 1) {
        if (WORK_U16(self->work, 0x67B4) != 0) {
            NameSlot29 *slots = (NameSlot29 *)((char *)D_089D712C + 0x39F38);
            NameSlot29 *slot = &slots[WORK_INT(self->work, 0x6858)];

            func_0887E81C((char *)&WORK_BYTE(self->work, 0x67B4), (char *)&WORK_BYTE(self->work, 0x67B4), 0x9C);
            func_089C5D98(date);
            NameProfile29 *profile = (NameProfile29 *)((char *)D_089D712C + 0x1FDD8);
            func_088C7900(slot, (char *)D_089D712C + 0x1FE08, (char *)&WORK_BYTE(self->work, 0x67B4), profile->id);
        } else {
            WORK_INT(self->work, 0x685C) = 1;
        }
    }
}
