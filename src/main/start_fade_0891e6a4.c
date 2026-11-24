// Queries a duration, starts a fade, registers the completion callback for the object and marks it busy.
struct S { char d[0x18]; int busy; };
extern int func_088D4C5C(void);
extern void func_08873DEC(int a, int b);
extern void func_08873D34(float t, int a, void *cb, struct S *s, unsigned char b, int c);
extern void func_0891E86C(void);
void func_0891E6A4(struct S *s)
{
    int t = func_088D4C5C();
    func_08873DEC(2, 2);
    func_08873D34((float)t, 2, func_0891E86C, s, 1, 2);
    s->busy = 1;
}
