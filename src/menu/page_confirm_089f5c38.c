typedef struct MenuPageState {
    char pad_0000[0x88];
    char field_88[0x400];
    char field_488[0x604];
    char field_A8C[0x3278 - 0xA8C];
    int field_3278;
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

extern int func_089FB568(MenuPageOwner *self, char *a, char *b, char *c);
extern void func_089FAE90(MenuPageOwner *self);

void func_089F5C38(MenuPageOwner *self) {
    MenuPageState *state = self->field_14C;
    if (!func_089FB568(self, state->field_88, state->field_488, state->field_A8C)) {
        return;
    }
    func_089FAE90(self);
    self->field_14C->field_3278 = 0xE;
}
