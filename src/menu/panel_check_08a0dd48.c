typedef struct MenuPanelState {
    char pad_0000[0xFD0];
    int field_FD0;
} MenuPanelState;

typedef struct MenuPanelOwner {
    char pad_0000[0x1320];
    MenuPanelState *field_1320;
} MenuPanelOwner;

extern int func_08A0C294(MenuPanelOwner *self);

void func_08A0DD48(MenuPanelOwner *self) {
    if (!func_08A0C294(self)) {
        return;
    }
    self->field_1320->field_FD0 = 0xF;
}
