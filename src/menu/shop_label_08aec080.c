typedef struct ShopState {
    char pad_0000[0x400];
    char field_400[0x1960 - 0x400];
    int field_1960;
    char pad_1964[8];
    int field_196C;
} ShopState;

typedef struct ShopOwner {
    char pad_0000[0x23F0];
    ShopState *field_23F0;
} ShopOwner;

extern unsigned short D_08AEFD5A[];
extern void func_088D3AE8(char *text, int id, int x, int y);

void func_08AEC080(ShopOwner *self)
{
    ShopState *state = self->field_23F0;
    int text = D_08AEFD5A[state->field_1960 - state->field_196C];
    func_088D3AE8(state->field_400, text, 0, 0x29);
}
