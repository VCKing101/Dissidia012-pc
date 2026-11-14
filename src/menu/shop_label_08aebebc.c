typedef struct ShopState {
    char pad_0000[0x400];
    char field_400[0x1934 - 0x400];
    int field_1934;
} ShopState;

typedef struct ShopOwner {
    char pad_0000[0x23F0];
    ShopState *field_23F0;
} ShopOwner;

extern unsigned short D_08AEFD36[];
extern void func_088D3AE8(char *text, int x, int id, int y);

void func_08AEBEBC(ShopOwner *self)
{
    ShopState *state = self->field_23F0;
    func_088D3AE8(state->field_400, 0x22, D_08AEFD36[state->field_1934], 0x27);
}
