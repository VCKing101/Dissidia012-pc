typedef struct MenuHelpState {
    char pad_000[0xD78];
    int field_D78;
} MenuHelpState;

typedef struct MenuHelpOwner {
    char pad_000[0xF64];
    MenuHelpState *field_F64;
} MenuHelpOwner;

extern void func_08A36898(MenuHelpOwner *self);
extern void func_08A36A94(MenuHelpOwner *self);

void func_08A36C84(MenuHelpOwner *self) {
    if (self->field_F64->field_D78 == 1) {
        func_08A36A94(self);
    } else {
        func_08A36898(self);
    }
}
