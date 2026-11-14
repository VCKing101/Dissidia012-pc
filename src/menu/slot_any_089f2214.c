typedef struct MarkSlot {
    unsigned char field_00;
    char pad_01[3];
    unsigned int field_04;
    char pad_08[0x7C];
} MarkSlot;

typedef struct MarkState {
    char pad_0000[0x29CC];
    MarkSlot field_29CC[9];
} MarkState;

typedef struct MarkOwner {
    char pad_000[0x14C];
    MarkState *field_14C;
} MarkOwner;

int func_089F2214(MarkOwner *self, int count)
{
    int i;
    int result = 0;
    for (i = 0; i < count; i++) {
        if (self->field_14C->field_29CC[i].field_00 == 1 && (self->field_14C->field_29CC[i].field_04 & 0x800) != 0) {
            result = 1;
            break;
        }
    }
    return result;
}
