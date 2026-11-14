/* Screen0B6A0: the value of item `index` for data entry `entry`: the
   entry's base value plus its percentage of the item's level (less
   `bonus` unless the items have flag 0x800) and of the kind bonus. */
typedef struct ValueItem29 {
    unsigned char kind_set;  /* +0x00: 1 when the items use kinds */
    char pad_01[0x3];
    unsigned int flags;      /* +0x04 */
    char pad_08[0x7];
    unsigned char kind;      /* +0x0F */
    char pad_10[0x4];
    short level;             /* +0x14 */
    short level_max;         /* +0x16 */
} ValueItem29;

typedef struct ValueEntry29 {
    short base;
    short percent;
    short bonus[9];
    char pad_16[0xA];
} ValueEntry29;

typedef struct ValueWork29 {
    char pad_00[0x50];
    ValueEntry29 *entries;   /* +0x50 */
} ValueWork29;

typedef struct ValueScreen29 {
    char pad_000[0x14C];
    ValueWork29 *work;
} ValueScreen29;

int func_089F050C(ValueScreen29 *self, ValueItem29 *items, short entry, unsigned char bonus,
                    int index, int kind)
{
    int value = 0;
    int item_kind = items[index].kind;

    if (kind != -1) {
        item_kind = kind;
    }
    if (entry >= 0) {
        ValueEntry29 *e = &self->work->entries[entry];
        value = e->base;
        if (value < 0) {
            value = 0;
        } else if (items->kind_set == 1) {
            int add = 0;
            short total;
            short part;
            if (item_kind < 9) {
                add = e->bonus[item_kind];
            }
            if ((items->flags & 0x800) != 0) {
                total = items[index].level_max + add;
            } else {
                total = items[index].level - bonus + add;
            }
            part = total * e->percent / 100;
            if (part > 0) {
                value = (short)(value + part);
            }
        }
    }
    return value;
}
