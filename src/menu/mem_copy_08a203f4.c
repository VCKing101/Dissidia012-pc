struct Src203F4 { int pad[4]; int a; int b; };
extern struct Src203F4 D_09B6D840;

void func_08A203F4(int param_1, int *param_2)
{
    struct Src203F4 *d = &D_09B6D840;
    param_2[1] = d->a;
    param_2[4] = d->b;
    param_2[5] = d->b;
}
