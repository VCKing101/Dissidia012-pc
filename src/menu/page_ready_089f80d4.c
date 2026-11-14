typedef struct MenuPageState {
    char pad_0000[0x345C];
    int field_345C;
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

extern void func_089F80F8(MenuPageOwner *self);

void func_089F80D4(MenuPageOwner *self) {
    self->field_14C->field_345C = 1;
    func_089F80F8(self);
}
