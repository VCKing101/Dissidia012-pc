typedef struct MenuSlotInfo {
    char pad_00[0xE];
    short field_0E;
} MenuSlotInfo;

typedef struct MenuSlotTable {
    char pad_0000[0x4C];
    MenuSlotInfo *field_4C;
    char pad_0050[0x2728 - 0x50];
    short field_2728;
} MenuSlotTable;

typedef struct MenuSlotOwner {
    char pad_000[0x14C];
    MenuSlotTable *field_14C;
} MenuSlotOwner;

short func_089F0230(MenuSlotOwner *self) {
    int value = self->field_14C->field_2728;
    short percent = self->field_14C->field_4C->field_0E;
    if (percent != -1) {
        value = value * percent / 100;
    }
    return (short)value;
}
