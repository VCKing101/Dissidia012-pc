typedef struct A6Ents {
    char pad_000[0x2B8];
    int field_2B8;
    char pad_2BC[0x4C];
    int field_308[1];
} A6Ents;

typedef struct A6EntsSelf {
    char pad_00[8];
    A6Ents *field_08;
    char field_0C[4];
} A6EntsSelf;

void func_08A94DE4(char *a, char *b, int c);

void func_08A8B7D8(A6EntsSelf *self) {
    A6Ents *r = self->field_08;
    int n = r->field_2B8;
    int i;
    for (i = 0; i < n; i++) {
        int v = *(int *)((char *)r + i * 4 + 0x308);
        if (v == 0) {
            func_08A94DE4(self->field_0C, (char *)r + 0x390, 0);
            r = self->field_08;
            n = r->field_2B8;
        } else if (v == 1) {
            func_08A94DE4(self->field_0C, (char *)r + 0x65C, 1);
            r = self->field_08;
            n = r->field_2B8;
        }
    }
}
