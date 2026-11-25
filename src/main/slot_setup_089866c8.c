typedef struct Tbl { int f0; int base; unsigned short *idx; } Tbl;
typedef struct O { char p0[0x8C]; char *recs; Tbl *tbl; char p1[0x1C]; void *pool; } O;
typedef struct S {
    char p0[0x1C]; void *m1c; char p1[4]; int sz24; char p2[0x08]; void *src30; int n34; void *src38;
    char p3[0x08]; void *m44; void *m48; char p4[0x70]; int cur; unsigned char flag;
} S;
extern void *func_08974A3C(void *pool, int n);
extern void func_08935524(void *d, void *s, int n);
extern int func_0896A114(int v);
extern void func_08987848(S *a, O *b, void *c);
extern void func_08987BBC(S *a, O *b);
extern void func_08987E60(S *a, O *b);
extern void func_089883E4(S *a, O *b);
int func_089866C8(S *self, O *other, void *c)
{
    Tbl *t = other->tbl;
    int v = *(int *)(other->recs + t->idx[t->base + self->cur] + 0x60);
    void *pool = other->pool;
    int n2 = self->n34 * 2;
    void *m = func_08974A3C(pool, self->sz24);
    if (m == 0) {
        return 0;
    }
    self->m1c = m;
    self->m44 = func_08974A3C(pool, n2);
    self->m48 = func_08974A3C(pool, n2);
    if (self->m44 == 0 || self->m48 == 0) {
        return 0;
    }
    func_08935524(self->m44, self->src30, n2);
    func_08935524(self->m48, self->src38, n2);
    if (func_0896A114(v) != 0) {
        func_08987848(self, other, c);
    } else {
        func_08987BBC(self, other);
    }
    func_08987E60(self, other);
    if (self->flag != 0) {
        func_089883E4(self, other);
    }
    return 1;
}
