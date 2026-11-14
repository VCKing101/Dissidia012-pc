/* Lists the slots that are used or past the loaded count. */
struct State08B2D994 {
    char pad_000[0x560];
    int field_560;
    char pad_564[0x578 - 0x564];
    unsigned char field_578[5];
    char pad_57D[0x6A4 - 0x57D];
    int field_6A4[6];
    int field_6BC;
};

struct Owner08B2D994 {
    char pad_000[0x844];
    struct State08B2D994 *field_844;
};

void func_08B2D994(struct Owner08B2D994 *self)
{
    int i;
    self->field_844->field_6BC = 0;
    for (i = 0; i < 5; i++) {
        if (self->field_844->field_578[i] != 0 || i >= self->field_844->field_560) {
            self->field_844->field_6A4[self->field_844->field_6BC] = i;
            self->field_844->field_6BC++;
        }
    }
}
