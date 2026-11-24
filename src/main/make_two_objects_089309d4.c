// Runs the init helper, creates an object from the table and its companion, then registers the group at +0xC/+0x44.
extern void func_08930AA0();
extern int func_0887AD78(void *, int, int, int, int, int);
extern int func_0887E534(int);
extern void func_088D428C(void *, void *, const void *, int);
extern char D_09B4E838[];
extern char D_089CF1A0[];
struct S { char pad[0xC]; char g[0x38]; int a; int b; };
void func_089309D4(struct S *self, int x) {
    struct S *s = self;
    int v = x;
    func_08930AA0();
    s->a = func_0887AD78(D_09B4E838, 0, v, 0, 0, 0);
    s->b = func_0887E534(s->a);
    func_088D428C(s->g, &s->a, D_089CF1A0, 1);
}
