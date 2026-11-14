typedef struct MenuSlotHeader {
    char pad_00[0x10];
    short field_10;
} MenuSlotHeader;

typedef struct MenuSlotRow {
    short field_00;
    short field_02;
    short field_04;
    char pad_06[0x4];
    short field_0A;
    short field_0C;
    short field_0E;
    char pad_10[0x10];
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

extern unsigned short D_08A0B23C[];

typedef struct MenuSlotRef {
    unsigned char field_00;
    signed char field_01;
} MenuSlotRef;

unsigned short func_089EF640(MenuSlotOwner *self, MenuSlotRef *ref) {
    unsigned short result = 0xFFFF;
    if (ref->field_00 == 3) {
        if (ref->field_01 < self->field_14C->field_40->field_10) {
            result = self->field_14C->field_44[ref->field_01].field_0A;
        }
    }
    return result;
}

unsigned short func_089EF688(MenuSlotOwner *self, signed char index) {
    unsigned short result = 0xFFFF;
    MenuSlotTable *table = self->field_14C;
    if (index < table->field_40->field_10) {
        result = table->field_44[index].field_0A;
    }
    return result;
}

unsigned short func_089EF6C4(MenuSlotOwner *self, signed char index) {
    unsigned short result = 0xFFFF;
    MenuSlotTable *table = self->field_14C;
    if (index < table->field_40->field_10) {
        result = table->field_44[index].field_0E;
    }
    return result;
}

unsigned short func_089EF700(MenuSlotOwner *self, MenuSlotRef *ref) {
    unsigned short result = 0xFFFF;
    if (ref->field_00 == 3) {
        if (ref->field_01 < self->field_14C->field_40->field_10) {
            result = self->field_14C->field_44[ref->field_01].field_0C;
        }
    }
    return result;
}

unsigned short func_089EF748(MenuSlotOwner *self, MenuSlotRef *ref) {
    unsigned short result = 0xFFFF;
    if (ref->field_00 == 3) {
        if (ref->field_01 < self->field_14C->field_40->field_10) {
            short kind = self->field_14C->field_44[ref->field_01].field_02;
            if (kind < 2) {
                result = D_08A0B23C[kind];
            }
        }
    }
    return result;
}

unsigned char func_089EF7AC(MenuSlotOwner *self, signed char index) {
    unsigned char result = 0;
    MenuSlotTable *table = self->field_14C;
    if (index < table->field_40->field_10) {
        result = table->field_44[index].field_02;
    }
    return result;
}
