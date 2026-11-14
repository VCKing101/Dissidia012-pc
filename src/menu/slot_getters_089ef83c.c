typedef struct MenuSlotHeader {
    char pad_00[0x10];
    short field_10;
} MenuSlotHeader;

typedef struct MenuSlotRow {
    short field_00;
    short field_02;
    short field_04;
    char pad_06[0x1A];
} MenuSlotRow;

typedef struct MenuSlotTable {
    char pad_00[0x40];
    MenuSlotHeader *field_40;
    MenuSlotRow *field_44;
} MenuSlotTable;

typedef struct MenuSlotOwner {
    char pad_000[0x14C];
    MenuSlotTable *field_14C;
} MenuSlotOwner;

typedef struct MenuSlotText {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
} MenuSlotText;

extern MenuSlotText D_08A0B240[];

unsigned short func_089EF83C(MenuSlotOwner *self, signed char index) {
    unsigned short result = 0x3407;
    MenuSlotTable *table = self->field_14C;
    if (index < table->field_40->field_10) {
        result = D_08A0B240[index].field_04;
    }
    return result;
}

unsigned char func_089EF880(MenuSlotOwner *self, signed char index) {
    unsigned char result = 0;
    MenuSlotTable *table = self->field_14C;
    if (index < table->field_40->field_10) {
        result = table->field_44[index].field_04;
    }
    return result;
}
