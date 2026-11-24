/* Sets the 0x100 word to 1 and the 0x126 byte to 0 on each of two sub-objects. */
typedef struct Sub { char pad_00[0x100]; int on; char pad_104[0x22]; unsigned char off; } Sub;
typedef struct Mid { char pad_00[0x5AC]; Sub *sub; } Mid;
typedef struct Top { char pad_00[0x38]; Mid *mid[2]; } Top;
extern Top *D_089D6A10;
void func_08B0E1BC(void)
{
    int i;
    for (i = 0; i < 2; i++) {
        Sub *s = D_089D6A10->mid[i]->sub;
        int zero = 0;
        s->on = 1;
        s->off = zero;
    }
}
