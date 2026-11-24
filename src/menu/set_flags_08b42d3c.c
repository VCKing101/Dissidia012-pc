/* Sets a word to 1, another to 0 and a byte flag at +0x2449 to 1. */
typedef struct S { int pad; int a; int b; char rest[0x2449 - 12]; char flag; } S;
void func_08B42D3C(S *self)
{
    int one = 1;
    self->a = one;
    self->b = 0;
    self->flag = 1;
}
