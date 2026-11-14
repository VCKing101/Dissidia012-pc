/* Draws the current stat row: its label, both values and its marker. */
struct State08B0D8CC {
    char pad_00000[0x404];
    char field_404[0xCB0 - 0x404];
    int field_CB0[5];
    int field_CC4[5];
    char pad_00CD8[0x24768 - 0xCD8];
    int field_24768;
};

struct Owner08B0D8CC {
    char pad_000[0x138];
    struct State08B0D8CC *field_138;
};

extern unsigned short D_08B1C6B6[];
extern void func_088D3AE8(char *layout, int slot, int text, int next);
extern void func_088D4584(char *layout, int slot, int field, int value);

void func_08B0D8CC(struct Owner08B0D8CC *self)
{
    int row = self->field_138->field_24768;
    func_088D3AE8(self->field_138->field_404, 0xE1, D_08B1C6B6[row], 0xEC);
    func_088D4584(self->field_138->field_404, row + 0xE2, 9, self->field_138->field_CC4[row]);
    func_088D4584(self->field_138->field_404, row + 0xE2, 10, self->field_138->field_CB0[row]);
    func_088D3AE8(self->field_138->field_404, row + 0xE2, 8, row + 0xE7);
}
