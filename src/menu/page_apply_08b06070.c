/* Applies the current page (once, unless `force` is clear) and marks it applied. */
struct State08B06070 {
    char pad_00000[0x1F89C];
    unsigned int field_1F89C;
    char pad_1F8A0[0x1F918 - 0x1F8A0];
    int field_1F918;
    char pad_1F91C[0x1F924 - 0x1F91C];
    int field_1F924;
};

struct Owner08B06070 {
    char pad_000[0x138];
    struct State08B06070 *field_138;
};

extern void func_08B1180C(struct Owner08B06070 *self, int page, int offset, int rows, unsigned char force);
extern void func_08B05DF8(struct Owner08B06070 *self);

void func_08B06070(struct Owner08B06070 *self, unsigned char force)
{
    if ((self->field_138->field_1F89C & 0x100) == 0 || force == 0) {
        int offset = self->field_138->field_1F918 - self->field_138->field_1F924;
        if (offset >= 0) {
            func_08B1180C(self, self->field_138->field_1F918, offset, 9, force);
            func_08B05DF8(self);
        }
        self->field_138->field_1F89C |= 0x100;
    }
}
