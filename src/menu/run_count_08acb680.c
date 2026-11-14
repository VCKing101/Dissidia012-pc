typedef struct RunState {
    char pad_0000[0xF98];
    int field_F98;
    char pad_F9C[8];
    int *field_FA4;
    char pad_FA8[0x10F4 - 0xFA8];
    int field_10F4;
} RunState;

typedef struct RunOwner {
    char pad_0000[0xDB30];
    RunState *field_DB30;
} RunOwner;

void func_08ACB680(RunOwner *self)
{
    int i;
    self->field_DB30->field_10F4 = 0;
    for (i = 0; i < self->field_DB30->field_F98; i++) {
        if (self->field_DB30->field_FA4[i] != self->field_DB30->field_10F4) {
            break;
        }
        self->field_DB30->field_10F4++;
    }
}
