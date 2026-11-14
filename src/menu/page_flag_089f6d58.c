typedef struct MenuPageState {
    char pad_0000[0x2724];
    unsigned int field_2724;
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

extern void func_089F65F0(MenuPageOwner *self);
extern int func_089F66E0(MenuPageOwner *self);
extern void func_089F6754(MenuPageOwner *self);

void func_089F6D58(MenuPageOwner *self) {
    if ((self->field_14C->field_2724 & 0x400000) != 0) {
        func_089F65F0(self);
    }
}

int func_089F6D8C(MenuPageOwner *self) {
    if (func_089F66E0(self) != 0) {
        func_089F6754(self);
        return 2;
    } else {
        return 1;
    }
}
