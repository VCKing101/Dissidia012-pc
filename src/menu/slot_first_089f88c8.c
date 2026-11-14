typedef struct MarkSlot {
    unsigned char field_00;
    char pad_01[0x83];
} MarkSlot;

typedef struct MarkState {
    char pad_0000[0x2730];
    signed char field_2730;
    signed char field_2731;
    char pad_2732[0x29CC - 0x2732];
    MarkSlot field_29CC[9];
} MarkState;

typedef struct MarkOwner {
    char pad_000[0x14C];
    MarkState *field_14C;
} MarkOwner;

void func_089F88C8(MarkOwner *self)
{
    int i;
    for (i = 0; i < self->field_14C->field_2731; i++) {
        if (self->field_14C->field_29CC[i].field_00 == 2) {
            self->field_14C->field_2730 = i + 1;
            break;
        }
    }
}
