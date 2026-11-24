// Runs the init helper, creates a type-0x19 object into +0x38, then registers the pair at +0x34/+0x38.
extern void func_0892FD70();
extern int func_0887B3EC(int, int, int, int, int, int);
extern void func_088D428C(void *, void *, const void *, int);
extern char D_089CF090[];
struct S { char pad[0x34]; char a[4]; int obj; };
void func_0892FCFC(struct S *self, int a, int b) {
    int y = b;
    struct S *s = self;
    int x = a;
    func_0892FD70();
    s->obj = func_0887B3EC(x, y, 0, 0x19, 0, 0);
    func_088D428C(s->a, &s->obj, D_089CF090, 1);
}
