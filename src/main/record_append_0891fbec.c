/* Appends a 0x34-byte record to the list (count at +0, records from +0x3C), pushing it through a stack copy, then runs the post step. */
typedef struct Rec { int w[13]; } Rec;
typedef struct S { int count; } S;
extern void func_08922680(void *dst, Rec *r);
extern void func_08922654(Rec *r, int n);
void func_0891FBEC(S *self, Rec *src)
{
    Rec tmp;
    char *dst;
    self->count = self->count + 1;
    dst = (char *)self + self->count * 0x34 + 8;
    tmp = *src;
    func_08922680(dst, &tmp);
    func_08922654(&tmp, 2);
}
