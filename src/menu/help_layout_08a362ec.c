typedef struct MenuHelpState {
    char pad_000[0xDB5];
    unsigned char field_DB5;
} MenuHelpState;

typedef struct MenuHelpOwner {
    char pad_000[0xF64];
    MenuHelpState *field_F64;
} MenuHelpOwner;

extern void *D_08A457EC;
extern void func_08A41454(void *global, short *layout);

void func_08A362EC(MenuHelpOwner *self, short *layout, int index) {
    if (self->field_F64->field_DB5 == 0 || index == 1) {
        func_08A41454(D_08A457EC, layout);
    } else {
        layout[0] = 0x14;
        layout[1] = 9;
        layout[2] = index + 9;
        layout[3] = 6;
        func_08A41454(D_08A457EC, layout + 4);
    }
}
