typedef struct MenuStatusState {
    char pad_0000[0x4B0];
    int field_4B0;
    int field_4B4;
    char pad_04B8[0x16B4 - 0x4B8];
    int field_16B4;
} MenuStatusState;

typedef struct MenuStatusOwner {
    char pad_0000[0x17B0];
    MenuStatusState *field_17B0;
} MenuStatusOwner;

extern unsigned short D_08A1B19C[];
extern void func_088DA0A8(int handle, unsigned short text, char *buffer, int *size);
extern void func_088D9B84(int handle, char *buffer);
extern void func_088D9AF4(int handle, int *size);

void func_08A18E08(MenuStatusOwner *self) {
    char buffer[64];
    int size;
    MenuStatusState *state = self->field_17B0;

    func_088DA0A8(state->field_4B4, D_08A1B19C[state->field_16B4], buffer, &size);
    func_088D9B84(self->field_17B0->field_4B0, buffer);
    func_088D9AF4(self->field_17B0->field_4B0, &size);
}
