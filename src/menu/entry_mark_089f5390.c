typedef struct MenuEntry {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    char pad_03;
    unsigned int field_04;
    char pad_08[0x7C];
} MenuEntry;

typedef struct MenuEntryTable {
    char pad_0000[0x29CC];
    MenuEntry field_29CC[9];
} MenuEntryTable;

typedef struct MenuEntryOwner {
    char pad_000[0x14C];
    MenuEntryTable *field_14C;
} MenuEntryOwner;

void func_089F5390(MenuEntryOwner *self, unsigned char index, unsigned char value) {
    self->field_14C->field_29CC[index].field_04 |= 0x80;
    self->field_14C->field_29CC[index].field_02 = value;
}
