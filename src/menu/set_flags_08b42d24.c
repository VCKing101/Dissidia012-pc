/* Sets two words to 1 and a byte flag at +0x2448 to 1. */
typedef struct S { int pad; int a; int b; char rest[0x2448 - 12]; char flag; } S;
void func_08B42D24(S *self)
{
    int one = 1;
    self->a = one;
    self->b = one;
    self->flag = 1;
}
