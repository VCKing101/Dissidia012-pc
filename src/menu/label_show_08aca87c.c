typedef struct LabelState {
    char pad_000[0x400];
    char field_400[0xB18];
    unsigned int field_F18;
    char pad_F1C[0xC];
    int field_F28;
    char pad_F2C[0x28];
    int field_F54;
} LabelState;

typedef struct LabelOwner {
    char pad_0000[0xDB30];
    LabelState *field_DB30;
} LabelOwner;

extern unsigned short D_08ADA276[][3];
extern void func_088D3AE8(char *text, int x, int id, int y);

void func_08ACA87C(LabelOwner *self)
{
    LabelState *state = self->field_DB30;
    if ((state->field_F18 & 0x40) != 0) {
        func_088D3AE8(state->field_400, 0x48, D_08ADA276[state->field_F28][state->field_F54], 0x20);
    }
}
