typedef struct MenuOrderState {
    char pad_000[0x880];
    unsigned int field_880;
    char pad_884[0xE54 - 0x884];
    int field_E54;
    int field_E58[6];
    int field_E70;
} MenuOrderState;

typedef struct MenuOrderOwner {
    char pad_000[0x14C];
    MenuOrderState *field_14C;
} MenuOrderOwner;

extern void func_088D46E4(unsigned int *list, int item, int first, int last, int flags, int count);

void func_08A41A9C(MenuOrderOwner *self) {
    int count = 0;
    int i;
    for (i = 0; i < 6; i++) {
        self->field_14C->field_E58[i] = i;
        count++;
    }
    self->field_14C->field_E54 = count;
}

void func_08A41AD4(MenuOrderOwner *self) {
    MenuOrderState *state = self->field_14C;
    unsigned int *list;

    func_088D46E4(&state->field_880, state->field_E70, 0, state->field_E54 - 1, 0, state->field_E54);
    list = &self->field_14C->field_880;
    *list |= 4;
}
