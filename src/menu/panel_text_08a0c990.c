typedef struct MenuPanelState {
    char pad_0000[0x4B0];
    int field_4B0;
    char pad_04B4[0x590 - 0x4B4];
    int field_590;
    char pad_0594[0x59C - 0x594];
    int field_59C;
    char pad_05A0[0x1070 - 0x5A0];
    int field_1070;
    char pad_1074[0x107C - 0x1074];
    int field_107C;
} MenuPanelState;

typedef struct MenuPanelOwner {
    char pad_0000[0x1320];
    MenuPanelState *field_1320;
} MenuPanelOwner;

extern unsigned short D_08A11DD0[];
extern void func_088DA0A8(int handle, unsigned short text, char *buffer, int *size);
extern void func_088D9B84(int handle, char *buffer);
extern void func_088D9AF4(int handle, int *size);

void func_08A0C990(MenuPanelOwner *self) {
    char buffer[64];
    int size;
    MenuPanelState *state = self->field_1320;

    func_088DA0A8(state->field_590, D_08A11DD0[state->field_1070 - state->field_107C], buffer, &size);
    func_088D9B84(self->field_1320->field_4B0, buffer);
    func_088D9AF4(self->field_1320->field_4B0, &size);
    func_088D9B84(self->field_1320->field_59C, buffer);
    func_088D9AF4(self->field_1320->field_59C, &size);
}
