typedef struct RangeEntry {
    short field_00;
    short field_02;
    short field_04;
    short field_06;
} RangeEntry;

typedef struct RangeTable {
    char pad_00[0x10];
    int field_10;
    char pad_14[0x5C];
    RangeEntry *field_70;
    short *field_74;
} RangeTable;

typedef struct RangeOwner {
    int field_00;
    RangeTable *field_04;
} RangeOwner;

int func_08A971F8(RangeOwner *self, int index, int *first, int *last)
{
    int slot;
    if (index >= self->field_04->field_10) {
        return 0;
    }
    slot = self->field_04->field_74[index];
    *first = self->field_04->field_70[slot].field_02;
    *last = self->field_04->field_70[slot].field_06;
    return 1;
}
