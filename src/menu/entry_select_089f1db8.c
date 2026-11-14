typedef struct MenuEntry {
    unsigned char field_00;
    char pad_01[3];
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

extern void func_089FF430(MenuEntryOwner *self, MenuEntry *entry, int mode, unsigned char flag);

void func_089F1DB8(MenuEntryOwner *self, unsigned char flag) {
    int i;
    for (i = 0; i < 9; i++) {
        if (self->field_14C->field_29CC[i].field_00 == 1
            && (self->field_14C->field_29CC[i].field_04 & 0x30) == 0) {
            func_089FF430(self, &self->field_14C->field_29CC[i], 1, flag);
        }
    }
}
