// Builds the particle quad vertex list for 30 particle slots and draws it.
extern void *func_08935524(void *d, const void *s, unsigned n);
extern void func_089A0384(int a);
extern void func_089A3A44(void *a, int b, float s);
extern void func_089A12A4(int a, int b, int c, int d, int e);
extern void func_0899D2F8(int a);
extern void *func_0885CD44(int n);
extern void func_089A3528(void *dst, void *a, void *b);
extern void func_089A34F4(void *dst, void *a, void *b);
extern void func_089A0520(int a, int b, int c, int d, void *p);
extern void func_089A03DC(int a);
extern const char D_08B5D5D0[];
extern const float D_08B5D5AC;
extern int D_089D5EC0;

typedef struct Row { float a, b, c, d; } Row;
typedef struct Part2 { float f0; float pad[3]; float pos[4]; float pad2[4]; unsigned char col; unsigned char kind; unsigned char pad3[14]; } Part2;
typedef struct Vtx { float a, b; unsigned col; float pos[3]; } Vtx;
typedef struct Obj { char pad[0x44]; Part2 *list; int x48; } Obj;

void func_08B5CA1C(Obj *o)
{
    Part2 *pf = o->list;
    char vec[16];
    Row tab[4];
    Vtx *buf, *p;
    int cnt, i;
    func_08935524(tab, D_08B5D5D0, 0x40);
    func_089A0384(7);
    func_089A3A44(vec, D_089D5EC0 + 2000, D_08B5D5AC);
    func_089A12A4(0, 2, 3, 0, 0);
    func_0899D2F8(o->x48);
    p = (Vtx *)func_0885CD44(0x5A0);
    buf = p;
    cnt = 0;
    i = 0;
    do {
        if (0.0f < pf->f0) {
            unsigned col = (unsigned)pf->col << 24 | 0x303030;
            func_089A3528(p[0].pos, pf->pos, vec);
            p[0].a = tab[pf->kind].a;
            p[0].b = tab[pf->kind].b;
            p[0].col = col;
            func_089A34F4(p[1].pos, pf->pos, vec);
            p[1].a = tab[pf->kind].c;
            cnt += 2;
            p[1].b = tab[pf->kind].d;
            p[1].col = col;
            p += 2;
        }
        i++;
        pf++;
    } while (i < 30);
    func_089A0520(6, 0x19F, cnt, 0, buf);
    func_089A03DC(7);
}
