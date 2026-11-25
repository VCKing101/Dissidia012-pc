/* Releases the page's per-slot objects (5 or 10 slots by flag) and the two shared objects, clearing each pointer. */
typedef struct Sl { char p[0xCF0]; int a; char p1[0xD18 - 0xCF4]; int b; char p2[0xD40 - 0xD1C]; int c; } Sl;
typedef struct Bk { char p[0xCE8]; int x0; int x1; } Bk;
typedef struct S { char p[0x138]; Bk *b; } S;
extern void func_0887BA5C(int h);
void func_08B10128(S *self, unsigned char small)
{
    int n = 10;
    int i;
    int off;
    Sl *e;
    if (small) {
        n = 5;
    }
    for (i = 0, off = 0; i < n; i++, off += 4) {
        e = (Sl *)((char *)self->b + off);
        if (e->a != 0) {
            func_0887BA5C(e->a);
            ((Sl *)((char *)self->b + off))->a = 0;
            e = (Sl *)((char *)self->b + off);
        }
        if (e->b != 0) {
            func_0887BA5C(e->b);
            ((Sl *)((char *)self->b + off))->b = 0;
            e = (Sl *)((char *)self->b + off);
        }
        if (e->c != 0) {
            func_0887BA5C(e->c);
            ((Sl *)((char *)self->b + off))->c = 0;
        }
    }
    if (self->b->x0 != 0) {
        func_0887BA5C(self->b->x0);
        self->b->x0 = 0;
    }
    if (self->b->x1 != 0) {
        func_0887BA5C(self->b->x1);
        self->b->x1 = 0;
    }
}
