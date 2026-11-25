#include "menu/agent105_types.h"
typedef struct Sub3128 { char pad[0xB20]; int cur; int f24; unsigned flags; } Sub3128;
typedef struct Self3128 { char pad[0x140]; Sub3128 *sub; } Self3128;
typedef struct Z3128 { char pad[0x30]; int f30; int f34; int f38; } Z3128;
typedef struct H3128 { char pad[0xC]; short k; short v; } H3128;
typedef struct Ob3128 { char pad[0xFC]; H3128 *p0; H3128 *p1; Z3128 *p2; } Ob3128;
typedef struct Hd3128 { char pad[0x144]; int h; } Hd3128;
extern Ent105 D_089E7434[];
extern void func_088D3EA0(int, void *, int);
extern void func_088D3CF0(int, void *, int);
extern void func_0887BA5C();
extern int func_089E2AB8(void *, int, int, int, int);
extern void func_088D9F5C(void *, int, int);
extern void func_088D428C(void *, void *, int, int);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_089E33A4(void *, void *);
extern void func_089E34F8(void *, int, void *, void *);

void func_089E3128(Self3128 *self, int idx, int a2, Ob3128 *a3, Hd3128 *t0)
{
    int flag = 0;
    short v;
    if (self->sub->cur != -1) {
        func_088D3EA0(a2, a3, D_089E7434[self->sub->cur].c);
        {
            Z3128 *z = a3->p2;
            z->f34 = 0;
            z->f30 = 0;
            z->f38 = 0;
        }
        if (t0->h != 0) {
            func_0887BA5C();
            t0->h = 0;
        }
        flag = 1;
        self->sub->cur = -1;
    }
    self->sub->cur = idx;
    self->sub->f24 = 0;
    if (t0->h != 0) {
        func_0887BA5C(t0->h);
        t0->h = 0;
    }
    t0->h = func_089E2AB8(self, 0, D_089E7434[self->sub->cur].a, 0, 0);
    func_088D3CF0(a2, a3, D_089E7434[self->sub->cur].c);
    {
    short w = 0x22;
    v = 7;
    if (a3->p0->k == 3) v = 4;
    if (!flag) {
        a3->p1->v = w;
        a3->p0->v = v;
    } else {
        func_088D9F5C(a3->p1, w, 1);
        func_088D9F5C(a3->p0, v, 1);
    }
    }
    func_088D428C(a3, t0, D_089E7434[self->sub->cur].d, 1);
    if (D_089E7434[self->sub->cur].b != 0xFFFF) {
        func_088D46E4(&self->sub->flags, D_089E7434[self->sub->cur].b, 0, 1, 0, -1);
        {
            unsigned *p = &self->sub->flags;
            *p |= 1;
        }
        func_089E33A4(self, a3);
    }
    func_089E34F8(self, a2, a3, t0);
}
