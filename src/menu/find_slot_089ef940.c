typedef struct SlotEntry {
    char pad_00[4];
    short field_04;
    char pad_06[0x1A];
} SlotEntry;

typedef struct SlotTable {
    char pad_00[0x10];
    short field_10;
} SlotTable;

typedef struct SlotCtx {
    char pad_00[0x40];
    SlotTable *field_40;
    SlotEntry *field_44;
} SlotCtx;

typedef struct SlotOwner {
    char pad_00[0x14C];
    SlotCtx *field_14C;
} SlotOwner;

int func_089EF940(SlotOwner *self, char key)
{
    int i = 0;
    int result = -1;
    int count = self->field_14C->field_40->field_10;
    if (i < count) {
        SlotEntry *e = self->field_14C->field_44;
        do {
            if (e->field_04 == key) {
                result = (char)i;
                break;
            }
            i++;
            e++;
        } while (i < count);
    }
    return result;
}
