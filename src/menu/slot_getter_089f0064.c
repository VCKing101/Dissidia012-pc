typedef struct MenuSlotInfo {
    char pad_00[0xC];
    short field_0C;
} MenuSlotInfo;

typedef struct MenuSlotTable {
    char pad_00[0x4C];
    MenuSlotInfo *field_4C;
} MenuSlotTable;

typedef struct MenuSlotOwner {
    char pad_000[0x14C];
    MenuSlotTable *field_14C;
} MenuSlotOwner;

unsigned short func_089F0064(MenuSlotOwner *self) {
    return self->field_14C->field_4C->field_0C;
}
