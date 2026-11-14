/* Screen0B6A0: computes the values of the item list: every row of a set
   (flag 0x800; the set's size from its data record, 5 by default) or the
   single first row. */
typedef struct ItemList29 {
    unsigned char kind_set;
    char pad_01[0x3];
    unsigned int flags;
    char pad_08[0x2];
    short set;           /* +0x0A */
} ItemList29;

typedef struct SetRecord29 {
    char pad_00[0x24];
} SetRecord29;

typedef struct SetInfo29 {
    char pad_00[0x2];
    short size;
    char pad_04[0x64];
} SetInfo29;

typedef struct ListWork29 {
    char pad_0000[0x60];
    SetInfo29 *infos;       /* +0x60 */
    char pad_0064[0xC];
    SetRecord29 *sets;      /* +0x70 */
    char pad_0074[0x3220];
    unsigned char bonus;    /* +0x3294 */
    char pad_3295[0x7];
    unsigned char set_bonus; /* +0x329C */
} ListWork29;

typedef struct ListScreen29 {
    char pad_000[0x14C];
    ListWork29 *work;
} ListScreen29;

extern void func_089F144C(ListScreen29 *self, ItemList29 *list, int index, unsigned char bonus);

void func_089F1580(ListScreen29 *self, ItemList29 *list)
{
    unsigned int flags = list->flags;
    ListWork29 *work = self->work;

    if ((flags & 0x800) != 0) {
        short info = *(short *)&work->sets[list->set].pad_00[0x10];
        int size = work->infos[info].size;
        int count = 5;
        int i;
        unsigned char bonus;
        if (size > 0) {
            count = size;
        }
        bonus = work->set_bonus;
        for (i = 0; i < count; i++) {
            func_089F144C(self, list, i, bonus);
        }
    } else {
        func_089F144C(self, list, 0, work->bonus);
    }
}
