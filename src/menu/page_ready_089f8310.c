typedef struct MenuPageState {
    char pad_0000[0x345C];
    int field_345C;
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

int func_089F8310(MenuPageOwner *self) {
    if (self->field_14C->field_345C == 0) {
        return 0;
    }
    return 1;
}
