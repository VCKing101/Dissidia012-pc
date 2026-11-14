typedef struct A6ChkInner {
    char pad_000[0x2A8];
    unsigned field_2A8;
    char pad_2AC[0x838];
    char field_AE4[4];
} A6ChkInner;

typedef struct A6ChkSelf {
    char pad_00[8];
    A6ChkInner *field_08;
} A6ChkSelf;

extern unsigned char D_089D5F20;
int func_088D4FBC(char *p);

int func_08A8E7E8(A6ChkSelf *self) {
    A6ChkInner *in = self->field_08;
    int r = 0;
    if ((in->field_2A8 & 4) != 0) {
        if (D_089D5F20 == 1) {
            r = 1;
        } else {
            if (func_088D4FBC(in->field_AE4) == 1) {
                r = 1;
            }
        }
    }
    return r;
}
