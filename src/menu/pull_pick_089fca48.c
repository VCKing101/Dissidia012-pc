#include "menu/agent29_types.h"

/* Screen0B6A0: fills the pick block for a pull: the current record id,
   the next pull number, and either the resumed pick (func_08A07394) or
   the chosen entry and its count (+1 on a first pull). */
typedef struct PullPick29 {
    int record;
    int number;
    unsigned short count;
    unsigned short entry;
} PullPick29;

typedef struct PullState29 {
    int field_00;
    int record;
} PullState29;

typedef struct PullInfo29 {
    char pad_00[0xB];
    char repeat;
} PullInfo29;

/* The entries at +0x29CC as this function reads them. */
typedef struct PickEntry29 {
    unsigned char kind;
    char pad_01[0x7];
    unsigned short count;
    char pad_0A[0x7A];
} PickEntry29;

typedef struct PickWork29 {
    char pad_0000[0x29CC];
    PickEntry29 entries[9];
} PickWork29;

extern PullState29 D_09B6DA40;
extern int func_088F2C10(void *table);
extern void func_08A07394(Screen0B6A0 *self, PullPick29 *pick);

void func_089FCA48(Screen0B6A0 *self, PullPick29 *pick, unsigned char resume)
{
    pick->record = D_09B6DA40.record;
    pick->number = func_088F2C10((char *)D_089D712C + 0x1E208);
    if (resume) {
        func_08A07394(self, pick);
    } else {
        pick->entry = WORK_U16(self->work, 0x272E);
        if (self->work->entries[self->work->field_2731].kind != 0) {
            pick->count = ((PickWork29 *)self->work)->entries[self->work->field_2731].count;
        } else if (((PullInfo29 *)WORK_INT(self->work, 0x6C))->repeat != 0) {
            pick->count = WORK_U16(self->work, 0x2746);
        } else {
            pick->count = WORK_U16(self->work, 0x2746) + 1;
        }
    }
}
