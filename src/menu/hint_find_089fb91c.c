typedef struct HintEntry {
    short field_00;
    short field_02;
    short field_04;
    unsigned short field_06;
} HintEntry;

typedef struct HintTable {
    char pad_00000[0x26D68];
    HintEntry field_26D68[0xC80];
    char pad_2D168[4];
    int field_2D16C;
} HintTable;

typedef struct HintState {
    char pad_0000[0x38A0];
    HintTable field_38A0;
} HintState;

typedef struct HintOwner {
    char pad_000[0x14C];
    HintState *field_14C;
} HintOwner;

int func_089FB91C(HintOwner *self, short id)
{
    HintTable *table = &self->field_14C->field_38A0;
    int result = 0;
    int i;
    for (i = 0; i < table->field_2D16C; i++) {
        if (table->field_26D68[i].field_06 == id) {
            result = i;
            break;
        }
    }
    return result;
}
