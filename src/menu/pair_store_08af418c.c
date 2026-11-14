/* Stores the two values in the state and in the current row, then refreshes,
   when func_08AF4054 allows it. */
struct Row08AF418C {
    char pad_00[0xB8];
    unsigned char field_B8;
    unsigned char field_B9;
    char pad_BA[0xC6 - 0xBA];
};

struct State08AF418C {
    char pad_0000[0xC80];
    struct Row08AF418C field_C80[(0x13E3 - 0xC80) / 0xC6];
    char pad_rows[0x13E3 - 0xC80 - ((0x13E3 - 0xC80) / 0xC6) * 0xC6];
    signed char field_13E3;
    char pad_13E4[0x2BFC - 0x13E4];
    int field_2BFC;
    int field_2C00;
};

struct Owner08AF418C {
    char pad_000[0x140];
    struct State08AF418C *field_140;
};

extern int func_08AF4054(struct Owner08AF418C *self);
extern void func_08AF3ECC(struct Owner08AF418C *self);

void func_08AF418C(struct Owner08AF418C *self, int first, int second)
{
    struct Row08AF418C *row;
    if (func_08AF4054(self) == 0) {
        return;
    }
    self->field_140->field_2BFC = first;
    self->field_140->field_2C00 = second;
    row = &self->field_140->field_C80[self->field_140->field_13E3];
    row->field_B8 = self->field_140->field_2BFC;
    row->field_B9 = self->field_140->field_2C00;
    func_08AF3ECC(self);
}
