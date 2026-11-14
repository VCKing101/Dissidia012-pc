typedef struct MenuChoiceCell {
    unsigned short field_00;
    unsigned short field_02;
    unsigned short field_04;
    unsigned short field_06;
} MenuChoiceCell;

typedef struct MenuChoiceTable {
    char pad_00[0x18];
    MenuChoiceCell field_18[16];
} MenuChoiceTable;

typedef struct MenuChoiceState {
    char pad_000[0x40C];
    char field_40C[0x38];
    int field_444[3];
    char pad_450[0x890 - 0x450];
    int field_890[7];
    int field_8AC;
    int field_8B0[3];
    int field_8BC[3];
    char pad_8C8[0xA88 - 0x8C8];
    int field_A88;
    char pad_A8C[0xAB4 - 0xA8C];
    int field_AB4;
    char pad_AB8[0xBE0 - 0xAB8];
    unsigned char field_BE0[4];
} MenuChoiceState;

typedef struct MenuChoiceOwner {
    char pad_000[0x14C];
    MenuChoiceState *field_14C;
} MenuChoiceOwner;

extern unsigned short D_08A4C002[];
extern MenuChoiceTable D_08A4BF7C;
extern void func_088D3AE8(char *list, int a, int b, int c);
extern void func_088D4584(char *list, int row, int column, int value);
extern void func_088D9F5C(int handle, int text, int mode);
extern void func_0887B924(int handle);
extern void func_0887B944(int handle);
extern void func_08A4A518(MenuChoiceOwner *self);

void func_08A4A3C4(MenuChoiceOwner *self) {
    func_088D3AE8(self->field_14C->field_40C, 3, D_08A4C002[self->field_14C->field_A88], 1);
    func_088D4584(self->field_14C->field_40C, self->field_14C->field_A88 + 4, 1, self->field_14C->field_8AC);
    func_088D4584(self->field_14C->field_40C, self->field_14C->field_A88 + 4, 2,
                  self->field_14C->field_890[self->field_14C->field_A88]);
}

void func_08A4A450(MenuChoiceOwner *self) {
    int i;
    for (i = 0; i < 3; i++) {
        int text = self->field_14C->field_BE0[i] != 0 ? 9 : 8;
        func_088D9F5C(self->field_14C->field_444[i], text, 1);
        func_0887B944(self->field_14C->field_8BC[i]);
    }
    {
        int selected = self->field_14C->field_AB4;
        if (self->field_14C->field_BE0[selected] != 0) {
            func_088D9F5C(self->field_14C->field_444[selected], 10, 1);
            func_0887B924(self->field_14C->field_8BC[selected]);
        }
    }
    func_08A4A518(self);
}

void func_08A4A518(MenuChoiceOwner *self) {
    MenuChoiceState *state = self->field_14C;
    int selected = state->field_AB4;
    char *list = state->field_40C;

    if (self->field_14C->field_BE0[selected] != 0) {
        func_088D4584(list, selected + 0xE, 0, state->field_8BC[selected]);
        func_088D4584(self->field_14C->field_40C, selected + 0xE, 1, self->field_14C->field_8B0[selected]);
        list = self->field_14C->field_40C;
    }
    func_088D3AE8(list, 0x11, D_08A4BF7C.field_18[selected].field_06, 0xD);
}
