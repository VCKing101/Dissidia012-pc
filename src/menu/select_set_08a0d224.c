typedef struct SelectInner {
    char pad_000[0xFEC];
    unsigned int field_FEC;
    char pad_FF0[0x106C - 0xFF0];
    int field_106C;
    int field_1070;
} SelectInner;

typedef struct SelectOwner {
    char pad_000[0x1320];
    SelectInner *field_1320;
} SelectOwner;

extern unsigned char func_088D4928(void *target, int value);
extern int func_08A1FAE4(int global, unsigned int key);
extern int D_08A23FF0;

void func_08A0D224(SelectOwner *self)
{
    int index = func_08A1FAE4(D_08A23FF0, self->field_1320->field_FEC);
    if (index >= 0 && self->field_1320->field_1070 != index) {
        func_088D4928(&self->field_1320->field_106C, index);
    }
}
