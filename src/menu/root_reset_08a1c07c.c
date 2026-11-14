typedef struct MenuRootState {
    char pad_0000[0x2BB8];
    int field_2BB8;
} MenuRootState;

typedef struct MenuRootOwner {
    char pad_000[0x154];
    MenuRootState *field_154;
} MenuRootOwner;

extern void func_08A1BF00(MenuRootOwner *self);
extern void func_08A1C0B8(MenuRootOwner *self);
extern void func_08A1C2F8(MenuRootOwner *self);

void func_08A1C07C(MenuRootOwner *self) {
    self->field_154->field_2BB8 = 0;
    func_08A1BF00(self);
    func_08A1C0B8(self);
    func_08A1C2F8(self);
}
