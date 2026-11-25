/* First set-up step of an element: runs the owner's hook, stores the pair, creates the two child objects and queries/stores their geometry. */
typedef struct Pmf { short adj; short pad; void (*fn)(void *); } Pmf;
typedef struct Tbl { char p0[0x10]; Pmf hook; } Tbl;
typedef struct S { char p0[4]; int a; int b; Tbl *tbl; int c0; int c1; } S;
extern char D_089CF2D8[];
extern int func_088DAA40(int a, int b, int c);
extern void func_088DA0A8(int a, int b, void *out0, void *out1);
extern void func_088D9B84(int h, void *v);
extern void func_088D9AF4(int h, void *v);
extern void func_088D3CF0(int a, void *b, char *c);
void func_08932090(S *self, int a, int b, int c, int d, int e)
{
    char m[0x40];
    int v;
    Pmf *p = &self->tbl->hook;
    p->fn((char *)self + p->adj);
    self->a = c;
    self->b = d;
    self->c0 = func_088DAA40(a, 10, 0);
    self->c1 = func_088DAA40(b, e, 0);
    func_088DA0A8(self->a, self->b, m, &v);
    func_088D9B84(self->c0, m);
    func_088D9AF4(self->c0, &v);
    func_088D3CF0(0, &self->c0, D_089CF2D8);
}
