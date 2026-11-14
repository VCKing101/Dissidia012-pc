typedef struct MenuPageState {
    char pad_0000[0x2728];
    short field_2728;
    char pad_272A[0x2744 - 0x272A];
    short field_2744;
    char pad_2746[0x279C - 0x2746];
    unsigned char field_279C;
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

void func_089F2F30(MenuPageOwner *self) {
    self->field_14C->field_279C &= ~1;
    self->field_14C->field_2728 = 0;
    self->field_14C->field_2744 = 0;
}
