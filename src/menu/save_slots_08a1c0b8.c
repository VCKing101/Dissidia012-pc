/* MenuRoot: rebuilds the root's save list from the save slots
   (D_089D712C +0x1FECC): clears the 25 records, then shows each slot
   with its four header values (zero when func_088F5618 cannot read
   them), and counts the "new save" place as one more. Standalone: the
   save list is not in the region header's RootWork. */
typedef struct SaveRecord29 {
    unsigned short flags;
    char pad_02[0x24A];
} SaveRecord29;

typedef struct SaveList29 {
    unsigned int flags;
    int count;                /* +0x04 */
    SaveRecord29 records[26]; /* +0x08 */
} SaveList29;

typedef struct SaveRootWork29 {
    char pad_0000[0x2BB8];
    SaveList29 list; /* +0x2BB8 */
} SaveRootWork29;

typedef struct SaveRoot29 {
    char pad_000[0x154];
    SaveRootWork29 *work;
} SaveRoot29;

typedef struct SaveSlot29 {
    char data[0x10A8];
} SaveSlot29;

typedef struct SaveSlots29 {
    SaveSlot29 slots[25];
    int count; /* +0x1A068 */
} SaveSlots29;

extern char *D_089D712C;
extern int func_088F5618(SaveSlots29 *slots, int *a, int *b, int *c, int *d, int slot);
extern void func_08A21530(SaveRoot29 *self, SaveSlot29 *slot, int a, int b, int c, int d, int index, short line,
                          int arg8);

void func_08A1C0B8(SaveRoot29 *self)
{
    int i;
    SaveSlots29 *slots;
    int a;
    int b;
    int c;
    int d;

    for (i = 0; i < 25; i++) {
        self->work->list.records[i].flags = 0;
    }
    slots = (SaveSlots29 *)(D_089D712C + 0x1FECC);
    self->work->list.count = slots->count;
    for (i = 0; i < self->work->list.count; i++) {
        SaveSlot29 *slot = &slots->slots[i];

        if (func_088F5618(slots, &a, &b, &c, &d, i) == 0) {
            a = 0;
            b = 0;
            c = 0;
            d = 0;
        }
        func_08A21530(self, slot, a, b, c, d, i, i, 0);
    }
    self->work->list.count++;
}
