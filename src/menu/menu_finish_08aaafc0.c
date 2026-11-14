typedef struct FinishState {
    int field_00;
    int field_04;
    char pad_008[0x5D0 - 0x8];
    int field_5D0;
    char pad_5D4[0x7CC0 - 0x5D4];
    unsigned int field_7CC0;
    char pad_7CC4[4];
    int field_7CC8;
    int field_7CCC;
} FinishState;

typedef struct FinishOwner {
    char pad_0000[0x7CFC];
    FinishState *field_7CFC;
} FinishOwner;

extern void func_08AB58E0(int menu, int kind, int value);
extern void func_08AB5790(int menu, int kind);

void func_08AAAFC0(FinishOwner *self)
{
    if ((self->field_7CFC->field_7CC0 & 2) == 0) {
        return;
    }
    if (self->field_7CFC->field_04 == 7) {
        func_08AB58E0(self->field_7CFC->field_7CCC, 9, self->field_7CFC->field_7CC8);
        func_08AB5790(self->field_7CFC->field_7CCC, 2);
    } else if (self->field_7CFC->field_04 == 3) {
        func_08AB58E0(self->field_7CFC->field_7CCC, 7, self->field_7CFC->field_5D0);
        func_08AB5790(self->field_7CFC->field_7CCC, 2);
    }
}
