typedef struct MenuPageState {
    char pad_0000[0x4D4];
    int field_4D4;
    char pad_04D8[0x4FC - 0x4D8];
    int field_4FC;
    char pad_0500[0x4];
    int field_504;
    char pad_0508[0x8];
    int field_510;
    char pad_0514[0x3374 - 0x514];
    int field_3374;
} MenuPageState;

typedef struct MenuPageOwner {
    char pad_000[0x14C];
    MenuPageState *field_14C;
} MenuPageOwner;

extern unsigned short D_08A0B328[];

extern void func_089F30E8(MenuPageOwner *self);
extern void func_088DA0A8(int handle, unsigned short text, char *buffer, int *size);
extern void func_088D9B84(int handle, char *buffer);
extern void func_088D9AF4(int handle, int *size);
extern void func_088D9F5C(int handle, int text, int mode);
extern void func_089F43D8(MenuPageOwner *self);

void func_089F309C(MenuPageOwner *self) {
    int text;

    func_089F30E8(self);
    text = self->field_14C->field_3374 == 0 ? 0x94 : 0x93;
    func_088D9F5C(self->field_14C->field_510, text, 1);
    func_089F43D8(self);
}

void func_089F30E8(MenuPageOwner *self) {
    char buffer[64];
    int size;
    MenuPageState *state = self->field_14C;
    unsigned short text = D_08A0B328[state->field_3374];

    if (state->field_3374 == 0) {
        func_088DA0A8(state->field_4FC, text, buffer, &size);
    } else {
        func_088DA0A8(state->field_4D4, text, buffer, &size);
    }
    func_088D9B84(self->field_14C->field_504, buffer);
    func_088D9AF4(self->field_14C->field_504, &size);
}
