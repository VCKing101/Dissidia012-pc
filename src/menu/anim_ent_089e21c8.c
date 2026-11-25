#include "menu/agent101_types.h"
typedef struct Wk { char pad8[8]; char b8[0x400]; void *objs[1]; char pad[0xA88-0x40C]; int cur; } Wk;
typedef struct Sc { char pad[0x140]; Wk *wk; } Sc;
extern void func_088DA0A8(int, int, void *, void *);
extern void func_088D9B84(void *, void *);
extern void func_088D9AF4(void *, void *);
extern void func_088D9F5C(void *, int, int);
extern void func_088D3EA0(void *, void *, int);
extern void func_088D4664(void *, void *, int);
extern void func_088D3F00(void *, void *, int);
extern void func_088D44EC(void *, void *, int);
#define OBJ(i) (((void **)((char *)self->wk + 0x408))[i])
int func_089E21C8(Sc *self)
{
    unsigned short idx;
    short kind;
    if (self->wk->cur < 0 || self->wk->cur >= 7)
        return 0;
    idx = D_089E7518[self->wk->cur].sA;
    if (idx == 0xFFFF)
        return 0;
    kind = *(short *)((char *)OBJ(idx) + 0xC);
    if (kind == -1)
        return 1;
    {
        char a[64];
        char b[4];
        int u, v;
        { unsigned short s6 = D_089E7518[self->wk->cur].s6; func_088DA0A8(*(int *)((char *)self->wk + 0x4D8), s6, a, b); }
        func_088D9B84(OBJ(idx), a);
        func_088D9AF4(OBJ(idx), b);
        u = D_089E7518[self->wk->cur].wC;
        v = D_089E7518[self->wk->cur].w10;
        if (kind == 0xE && (unsigned char)((*(unsigned short *)((char *)OBJ(idx) + 0x20) & 0x20) != 0)) {
            func_088D9F5C(OBJ(idx), -1, 1);
            func_088D3EA0((char *)self->wk + 8, (char *)self->wk + 0x408, u);
            func_088D4664((char *)self->wk + 0x408, (char *)self->wk + 0x8B8, v);
        }
        func_088D3F00((char *)self->wk + 8, (char *)self->wk + 0x408, u);
        func_088D44EC((char *)self->wk + 0x408, (char *)self->wk + 0x8B8, v);
        return 0;
    }
}
