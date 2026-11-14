#include "menu/agent29_types.h"

/* Screen1AB08: writes the options screen's choices back into the option
   block (+0x652C of the root's work): the first list's item, then for the
   bank page either the chosen amount and item (mode 0) or the kind and
   the five items (mode 1), and the chosen entry (-1 for none); then
   applies them and saves. */
typedef struct OptionSlot29 {
    short amount;
    short item;
    char pad_04[0x34];
} OptionSlot29;

typedef struct Options29 {
    unsigned short flags;
    char pad_02[0x16];
    short item;           /* +0x18 */
    char pad_1A[0xBA];
    int mode;             /* +0xD4 */
    short kind;           /* +0xD8 */
    short entry;          /* +0xDA */
    char pad_DC[0x1C];
    OptionSlot29 slots[5]; /* +0xF8 */
} Options29;

void func_08A18B20(Screen1AB08 *self)
{
    Options29 *options = (Options29 *)((char *)D_08A23FF0->work + 0x652C);
    int i;

    options->item = (&WORK_SHORT(self->work, 0x1140))[WORK_INT(self->work, 0x1680)];
    if (self->work->field_10F8 == 0) {
        options->mode = 0;
        options->slots[0].amount = (&WORK_INT(self->work, 0x1540))[WORK_INT(self->work, 0x16A0) * 2];
        options->slots[0].item = WORK_INT(self->work, 0x16A8);
        options->kind = -1;
    } else {
        options->mode = 1;
        options->kind = (&WORK_INT(self->work, 0x10FC))[(unsigned int)WORK_INT(self->work, 0x1690) - 1];
        for (i = 0; i < 5; i++) {
            options->slots[i].item = (&WORK_INT(self->work, 0x1110))[i];
        }
    }
    if (WORK_INT(self->work, 0x1698) != 0) {
        options->entry = (&WORK_INT(self->work, 0x1658))[WORK_INT(self->work, 0x1698) - 1];
    } else {
        options->entry = -1;
    }
    func_08A1D59C(D_08A23FF0);
    func_088ACA10(D_089D712C);
}
