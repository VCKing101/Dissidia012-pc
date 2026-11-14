typedef struct MenuHelpState {
    char pad_000[0xD94];
    unsigned int field_D94;
} MenuHelpState;

typedef struct MenuHelpOwner {
    char pad_000[0xF64];
    MenuHelpState *field_F64;
} MenuHelpOwner;

extern void func_08A38034(MenuHelpOwner *self);
extern void func_08A36C84(MenuHelpOwner *self);

void func_08A3764C(MenuHelpOwner *self) {
    if ((self->field_F64->field_D94 & 1) != 0) {
        return;
    }
    func_08A38034(self);
    func_08A36C84(self);
}
