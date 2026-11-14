typedef struct MenuRootState {
    char pad_000[0xD7C];
    int field_D7C;
    int field_D80;
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

extern void func_08A1C07C(MenuRootOwner *self);

void func_08A1B42C(MenuRootOwner *self) {
    self->field_154->field_D80 = 0;
    func_08A1C07C(self);
    self->field_154->field_D7C = 1;
}
