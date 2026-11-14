/* Draws the marks of the current row (two values, the selected arrow) and
   its title. */
struct State08B0E0DC {
    char pad_00000[0x404];
    char field_404[0xCF0 - 0x404];
    int field_CF0[10];
    int field_D18[10];
    char pad_00D40[0x24814 - 0xD40];
    int field_24814;
};

struct Owner08B0E0DC {
    char pad_000[0x138];
    struct State08B0E0DC *field_138;
};

extern unsigned short D_08B1C8D0[];
extern int func_08B0E058(struct Owner08B0E0DC *self);
extern void func_088D4584(char *layout, int slot, int field, int value);
extern void func_088D3AE8(char *layout, int slot, int text, int next);

void func_08B0E0DC(struct Owner08B0E0DC *self)
{
    int row = func_08B0E058(self);
    if (self->field_138->field_D18[row] != 0) {
        func_088D4584(self->field_138->field_404, row + 0xEE, 0, self->field_138->field_D18[row]);
    }
    if (self->field_138->field_CF0[row] != 0) {
        func_088D4584(self->field_138->field_404, row + 0xEE, 1, self->field_138->field_CF0[row]);
    }
    if (row == self->field_138->field_24814) {
        func_088D3AE8(self->field_138->field_404, row + 0xEE, 2, row + 0xF8);
    }
    func_088D3AE8(self->field_138->field_404, 0xED, D_08B1C8D0[row], 0x102);
}
