// Runs the init helper, creates an object of the given type into +0x34, then registers the pair at +0x30/+0x34.
extern void func_089302A4();
extern int func_0887B3EC(int, int, int, int, int, int);
extern void func_088D428C(void *, void *, const void *, int);
extern char D_089CF148[];
struct S { char pad[0x30]; char a[4]; int obj; };
void func_08930224(struct S *self, int a, int b, int c) {
    int z = c;
    int y = b;
    struct S *s = self;
    int x = a;
    func_089302A4();
    s->obj = func_0887B3EC(x, y, 0, z, 0, 0);
    func_088D428C(s->a, &s->obj, D_089CF148, 1);
}
