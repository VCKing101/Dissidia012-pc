typedef struct MarkSlot {
    unsigned char field_00;
    char pad_01[3];
    unsigned int field_04;
    char pad_08[6];
    short field_0E;
    char pad_10[0x74];
} MarkSlot;

typedef struct MarkState {
    char pad_0000[0x29CC];
    MarkSlot field_29CC[9];
} MarkState;

typedef struct MarkOwner {
    char pad_000[0x14C];
    MarkState *field_14C;
} MarkOwner;

int func_089F0B98(MarkOwner *self, short id)
{
    MarkState *state = self->field_14C;
    int result = 0;
    int i;
    for (i = 0; i < 9; i++) {
        if (state->field_29CC[i].field_00 == 2 && (state->field_29CC[i].field_04 & 0x1000) != 0 &&
            state->field_29CC[i].field_0E == id) {
            result = 1;
            break;
        }
    }
    return result;
}
