typedef struct A6Src {
    char pad_00[4];
    unsigned char field_04;
} A6Src;

typedef struct A6FlagOwner {
    char pad_00[0x4C];
    unsigned field_4C;
    int field_50;
    char pad_54[0xD4];
    A6Src *field_128[4];
} A6FlagOwner;

void func_08A67340(A6FlagOwner *self) {
    A6Src *src = self->field_128[self->field_50];
    if (src->field_04 != 0) {
        self->field_4C |= 4;
        return;
    } else {
        self->field_4C &= ~4;
        return;
    }
}
