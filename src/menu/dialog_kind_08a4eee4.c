typedef struct DialogState {
    char pad_0000[0x6FF8];
    int field_6FF8;
} DialogState;

typedef struct DialogOwner {
    char pad_00[0x5C];
    DialogState *field_5C;
} DialogOwner;

extern void func_08A697B0(int dialog, int kind, int value);

void func_08A4EEE4(DialogOwner *self, int kind)
{
    switch (kind) {
    default:
        func_08A697B0(self->field_5C->field_6FF8, 6, -1);
        break;
    case 1:
        func_08A697B0(self->field_5C->field_6FF8, 7, -1);
        break;
    case 2:
        func_08A697B0(self->field_5C->field_6FF8, 8, -1);
        break;
    }
}
