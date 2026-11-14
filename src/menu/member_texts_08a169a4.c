#include "menu/agent29_types.h"

/* Screen181E0: remakes (show == 1) or hides the ten member texts
   (0x2E..) of the save in place +0xC7C of the root's save list: for the
   page in view (+0xC98 less one when +0xC90 is 1), a text (message 0x5C)
   for each member set on it, packed from the first text. */
typedef struct SaveMembers29 {
    char pad_000[0x11C];
    short members[8][0x1C]; /* +0x11C */
} SaveMembers29;

typedef struct MemberList29 {
    char pad_0000[0x2BC0];
    char records[26][0x24C]; /* +0x2BC0 */
} MemberList29;

extern void *func_08A1D42C(MenuRoot *root, int kind, int message, int *args, int count);

void func_08A169A4(Screen181E0 *self, unsigned char show)
{
    int page = 0;
    SaveMembers29 *record = (SaveMembers29 *)((MemberList29 *)D_08A23FF0->work)->records[self->work->field_C7C];
    int i;
    int n;
    int args[1];

    if (WORK_INT(self->work, 0xC90) == 1) {
        page = WORK_INT(self->work, 0xC98) - 1;
        if (page < 0) {
            page = 0;
        }
    }
    if (show == 1) {
        n = 0;
        for (i = 0; i < 10; i++) {
            if (self->work->texts[0x2E + i] != 0) {
                func_0887BA5C(self->work->texts[0x2E + i]);
                self->work->texts[0x2E + i] = 0;
            }
            if (record->members[page][i] != -1) {
                args[0] = record->members[page][i];
                self->work->texts[0x2E + n] = func_08A1D42C(D_08A23FF0, 0, 0x5C, args, 1);
                n++;
            }
        }
    } else {
        for (i = 0; i < 10; i++) {
            if (self->work->texts[0x2E + i] != 0) {
                func_0887B944(self->work->texts[0x2E + i]);
            }
        }
    }
}
