typedef struct ShopState {
    char pad_0000[0x400];
    char field_400[0x1924 - 0x400];
    unsigned int field_1924;
    char pad_1928[0x1960 - 0x1928];
    int field_1960;
    char pad_1964[8];
    int field_196C;
} ShopState;

typedef struct ShopOwner {
    char pad_0000[0x23F0];
    ShopState *field_23F0;
} ShopOwner;

extern unsigned short D_08AEFD3E[];
extern void func_088D3AE8(char *text, int x, int id, int y);
extern void func_08AEC080(ShopOwner *self);

void func_08AEBF6C(ShopOwner *self)
{
    ShopState *state = self->field_23F0;
    func_088D3AE8(state->field_400, 0x21, D_08AEFD3E[state->field_1960 - state->field_196C], 0x28);
    if ((self->field_23F0->field_1924 & 0x2000) != 0) {
        func_08AEC080(self);
    }
}
