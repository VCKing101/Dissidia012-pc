/* Sets the page label (text 0x3C) for the page offset and redraws the frame. */
struct State08B077BC {
    char pad_00000[0x404];
    char field_404[0x1F918 - 0x404];
    int field_1F918;
    char pad_1F91C[0x1F924 - 0x1F91C];
    int field_1F924;
};

struct Owner08B077BC {
    char pad_000[0x138];
    struct State08B077BC *field_138;
};

extern unsigned short D_08B1C5E8[];
extern void func_088D3AE8(char *layout, int slot, int text, int next);
extern void func_08B051F0(struct Owner08B077BC *self);

void func_08B077BC(struct Owner08B077BC *self)
{
    func_088D3AE8(self->field_138->field_404, 0x3C,
                  D_08B1C5E8[self->field_138->field_1F918 - self->field_138->field_1F924], 0x21);
    func_08B051F0(self);
}
