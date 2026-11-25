extern void func_0881A284(void);
extern int func_08899300(void);
extern int func_088988B4(int);
extern void *func_08842100(int);

typedef struct P8 { int a, b; } P8;
typedef struct E2 { int a; int b; unsigned char f; } E2;
typedef struct E4 { int a, b, c, d, e; } E4;
typedef struct S4 { int p0, p1, i8, ic, p10, i14; } S4;
typedef struct S {
    unsigned char init; int n1; int *a1; int n2; E2 *a2; int n3; int **a3; int n4; E4 *a4;
} S;
typedef struct Src {
    char pad0[0x94]; int n2; P8 *s2; int pad9c; int *t2; int padA4; int n3; char *s3; int padB0; int n4; S4 *s4; int padBC, padC0; P8 *t4; int padC8, padCC; P8 *u4;
} Src;
extern Src *D_089D4910;

int func_0881AA10(S *self)
{
    int i;
    Src *src;
    int n;
    int v;
    func_0881A284();
    v = func_08899300();
    src = D_089D4910;
    self->n1 = func_088988B4(v);
    n = self->n1;
    if (n != 0) {
        self->a1 = (int *)func_08842100(n << 2);
        if (self->a1 == 0) return -1;
        { int j = 0;
        for (; j < self->n1; j++) self->a1[j] = 1; }
    }
    n = src->n2;
    self->n2 = n;
    if (n != 0) {
        self->a2 = (E2 *)func_08842100(n * 12);
        if (self->a2 == 0) return -2;
        { int i = 0;
        for (; i < self->n2; i++) {
            P8 *p = &src->s2[i];
            self->a2[i].a = p->b;
            self->a2[i].b = (int)(src->t2 + p->a);
            self->a2[i].f = 1;
        } }
    }
    n = src->n3;
    self->n3 = n;
    if (n != 0) {
        self->a3 = (int **)func_08842100(n << 2);
        if (self->a3 == 0) return -3;
        { int i = 0;
        for (; i < self->n3; i++) self->a3[i] = (int *)(src->s3 + i * 16); }
    }
    n = src->n4;
    self->n4 = n;
    if (n != 0) {
        self->a4 = (E4 *)func_08842100(n * 20);
        if (self->a4 == 0) return -4;
        { int i = 0;
        for (; i < self->n4; i++) {
            S4 *p = &src->s4[i];
            self->a4[i].a = (int)p;
            self->a4[i].b = p->ic;
            self->a4[i].c = (int)(src->t4 + p->i8);
            self->a4[i].d = p->i14;
            self->a4[i].e = (int)(src->u4 + p->p10);
        } }
    }
    self->init = 1;
    return 0;
}
