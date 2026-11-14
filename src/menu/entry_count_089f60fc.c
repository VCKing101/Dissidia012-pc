typedef struct MenuEntry {
    char data[0x84];
} MenuEntry;

typedef struct MenuEntryTable {
    char pad_0000[0x2731];
    signed char field_2731;
    char pad_2732[0x29CC - 0x2732];
    MenuEntry field_29CC[9];
} MenuEntryTable;

typedef struct MenuEntryOwner {
    char pad_000[0x14C];
    MenuEntryTable *field_14C;
} MenuEntryOwner;

extern int func_089F60B4(MenuEntryOwner *self, MenuEntry *entry);

int func_089F60FC(MenuEntryOwner *self) {
    int i;
    int count = 0;

    for (i = 0; i < self->field_14C->field_2731; i++) {
        if (func_089F60B4(self, &self->field_14C->field_29CC[i]) != 0) {
            count++;
        }
    }
    return count;
}
