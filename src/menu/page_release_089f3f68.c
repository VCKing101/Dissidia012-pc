typedef struct MenuPageState {
    char pad_000[0x488];
    int field_488[16];
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

extern void func_088D9CFC(int handle);

void func_089F3F68(MenuPageOwner *self, unsigned short base) {
    int i;
    for (i = 1; i < 5; i++) {
        func_088D9CFC(self->field_14C->field_488[base + i]);
    }
}
