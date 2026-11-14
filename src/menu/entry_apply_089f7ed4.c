typedef struct MenuEntry {
    unsigned char field_00;
    unsigned char field_01;
    unsigned char field_02;
    char pad_03;
    unsigned int field_04;
    char pad_08[0x4];
    unsigned char field_0C;
    char pad_0D;
    short field_0E;
    char pad_10[0x4];
    short field_14;
    char pad_16[0x6E];
} MenuEntry;

typedef struct MenuEntryTable {
    char pad_0000[0x2734];
    unsigned short field_2734;
    char pad_2736[0x29CC - 0x2736];
    MenuEntry field_29CC[9];
    char pad_2E70[0x3444 - 0x2E70];
    int field_3444;
    int field_3448;
    int field_344C;
} MenuEntryTable;

typedef struct MenuEntryOwner {
    char pad_000[0x14C];
    MenuEntryTable *field_14C;
} MenuEntryOwner;

extern int func_089F2FF0(MenuEntryOwner *self);
extern void func_089F1E98(MenuEntryOwner *self, MenuEntry *entry);
extern void func_089FAF14(MenuEntryOwner *self, int mode, int value);

int func_089F7ED4(MenuEntryOwner *self) {
    int result = 0;
    int index = func_089F2FF0(self);

    if (self->field_14C->field_29CC[index].field_14 <= self->field_14C->field_2734) {
        unsigned char kind;
        func_089F1E98(self, &self->field_14C->field_29CC[index]);
        kind = self->field_14C->field_29CC[index].field_0C;
        result = kind == 1 ? 0 : kind == 2 ? 1 : kind == 3 ? 2 : 0;
        func_089FAF14(self, result, self->field_14C->field_29CC[index].field_0E);
        result = 1;
        self->field_14C->field_3444 = 0;
        self->field_14C->field_3448 = 0;
        self->field_14C->field_344C = 0;
    }
    return result;
}
