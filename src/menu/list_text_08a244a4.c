typedef struct MenuListState {
    char pad_0000[0x48C];
    int field_48C;
    int field_490;
    char pad_0494[0x2000 - 0x494];
    int field_2000;
    char pad_2004[0x8];
    int field_200C;
} MenuListState;

typedef struct MenuListOwner {
    char pad_0000[0x28F0];
    MenuListState *field_28F0;
} MenuListOwner;

extern unsigned short D_08A28AD0[];
extern void func_088DA0A8(int handle, unsigned short text, char *buffer, int *size);
extern void func_088D9B84(int handle, char *buffer);
extern void func_088D9AF4(int handle, int *size);

void func_08A244A4(MenuListOwner *self) {
    char buffer[64];
    int size;
    MenuListState *state = self->field_28F0;

    func_088DA0A8(state->field_48C, D_08A28AD0[state->field_2000 - state->field_200C], buffer, &size);
    func_088D9B84(self->field_28F0->field_490, buffer);
    func_088D9AF4(self->field_28F0->field_490, &size);
}
