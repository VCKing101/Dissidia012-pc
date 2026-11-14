/* Clears the count and the three value arrays. */
struct Table088C53A0 { int count; int a[32]; int b[32]; int c[2]; };
extern "C" {
void func_088C53A0(Table088C53A0 *t)
{
    t->count = 0;
    for (int i = 0; i < 32; i++) {
        t->a[i] = 0;
    }
    for (int i = 0; i < 32; i++) {
        t->b[i] = 0;
    }
    for (int i = 0; i < 2; i++) {
        t->c[i] = 0;
    }
}
}
