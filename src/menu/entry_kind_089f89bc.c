typedef struct MenuEntry {
    unsigned char field_00;
    char pad_01[0x83];
} MenuEntry;

typedef struct MenuEntryTable {
    char pad_0000[0x29CC];
    MenuEntry field_29CC[9];
} MenuEntryTable;

typedef struct MenuEntryOwner {
    char pad_000[0x14C];
    MenuEntryTable *field_14C;
} MenuEntryOwner;

extern int func_089F2FF0(MenuEntryOwner *self);

int func_089F89BC(MenuEntryOwner *self) {
    MenuEntryTable *table = self->field_14C;
    MenuEntry *entry = &table->field_29CC[func_089F2FF0(self)];
    if (entry->field_00 == 6) {
        return 1;
    }
    return 0;
}
