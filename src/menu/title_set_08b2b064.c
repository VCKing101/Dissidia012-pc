/* Sets the title text (slot 0x12) for the current mode. */
struct State08B2B064 {
    char pad_000[0x1C];
    char field_01C[0x748 - 0x1C];
    int field_748;
};

struct Owner08B2B064 {
    char pad_000[0xA18];
    struct State08B2B064 *field_A18;
};

extern unsigned short D_08B2D1D4[];
extern void func_088D3AE8(char *layout, int slot, int text, int next);

void func_08B2B064(struct Owner08B2B064 *self)
{
    func_088D3AE8(self->field_A18->field_01C, 0x12, D_08B2D1D4[self->field_A18->field_748], 0x14);
}
