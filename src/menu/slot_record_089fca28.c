typedef struct MenuSlotRecord {
    char data[0x30];
} MenuSlotRecord;

typedef struct MenuSlotTable {
    char pad_00[0x34];
    MenuSlotRecord *field_34;
} MenuSlotTable;

typedef struct MenuSlotOwner {
    char pad_000[0x14C];
    MenuSlotTable *field_14C;
} MenuSlotOwner;

MenuSlotRecord *func_089FCA28(MenuSlotOwner *self, unsigned short index) {
    return &self->field_14C->field_34[index];
}
