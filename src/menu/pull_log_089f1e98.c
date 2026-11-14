#include "menu/agent29_types.h"

/* Screen0B6A0: records a pull result: counts it (+0x27AC); a pick with
   flag 0x10000 only goes to the save's list at +0xF6E0; otherwise the
   pick is applied to the save and logged in the 128-entry history ring
   (+0x27CC, start +0x27C4, count +0x27C8), and a kind 1 pick is added
   to the collection (+0x1E208). */
typedef struct Pick29 {
    char pad_00[0x4];
    unsigned int flags;   /* +0x04 */
    char pad_08[0x4];
    unsigned char kind;   /* +0x0C */
    char pad_0D[0x1];
    short id;             /* +0x0E */
    char pad_10[0x8];
    char data[0x1];       /* +0x18 */
} Pick29;

typedef struct PickLog29 {
    unsigned char kind;
    char pad_01[0x1];
    short id;
} PickLog29;

typedef struct PickWork29 {
    char pad_0000[0x27AC];
    int pulls;            /* +0x27AC */
    char pad_27B0[0x14];
    int start;            /* +0x27C4 */
    int count;            /* +0x27C8 */
    PickLog29 log[0x80];  /* +0x27CC */
} PickWork29;

typedef struct PickApply29 {
    int field_00;
    char data[0x17C];
} PickApply29;

extern void func_088B8E58(void *list, int id);
extern void func_088C1C0C(PickApply29 *apply);
extern void func_088C1F6C(PickApply29 *apply, char *data, void *save, char *pick, int record);
extern void func_088F2B84(void *collection, int id);

void func_089F1E98(Screen0B6A0 *self, Pick29 *pick)
{
    PickApply29 apply;
    char *save;
    int slot;

    ((PickWork29 *)self->work)->pulls++;
    if (pick->flags & 0x10000) {
        func_088B8E58((char *)D_089D712C + 0xF6E0, pick->id);
    } else {
        save = (char *)D_089D712C;
        func_088C1C0C(&apply);
        func_088C1F6C(&apply, apply.data, save, pick->data, WORK_INT(self->work, 0x328C));
        slot = ((PickWork29 *)self->work)->start + ((PickWork29 *)self->work)->count;
        if (slot >= 0x80) {
            slot -= 0x80;
        }
        if (((PickWork29 *)self->work)->count < 0x80) {
            ((PickWork29 *)self->work)->count++;
        } else {
            ((PickWork29 *)self->work)->start++;
            if (((PickWork29 *)self->work)->start >= 0x80) {
                ((PickWork29 *)self->work)->start = 0;
            }
        }
        ((PickWork29 *)self->work)->log[slot].kind = pick->kind;
        ((PickWork29 *)self->work)->log[slot].id = pick->id;
        if (((PickWork29 *)self->work)->log[slot].kind == 1) {
            func_088F2B84((char *)D_089D712C + 0x1E208, ((PickWork29 *)self->work)->log[slot].id);
        }
    }
}
