struct Dest203D8 { int pad[4]; int a; int b; };
extern struct Dest203D8 D_09B6D840;

void func_08A203D8(int param_1, int *param_2)
{
    struct Dest203D8 *d = &D_09B6D840;
    d->a = param_2[1];
    d->b = param_2[4];
}
