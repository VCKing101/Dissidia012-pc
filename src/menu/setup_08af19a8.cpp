extern "C" {
typedef struct { char pad[0x30]; int x30; } G;
typedef struct { char p0[0x1C]; char a[1]; char p1[0x41C - 0x1D]; char b[1]; char p2[0x8A0 - 0x41D]; int h; char p3[4]; char c[1]; char p4[0xB38 - 0x8A9]; int r; int s; unsigned flags; char p5[0xB6C - 0xB44]; unsigned short k0, k1, k2, k3; } Blk;
typedef struct { char p0[0x140]; Blk *blk; } Obj;
struct H { static unsigned char on(G *g) { return g->x30 != 0; } static void put(Obj *o, int v) { o->blk->r = v; } };

extern G *D_089D7F90;
extern char D_08B00176[], D_08B001D6[], D_08B00366[], D_08B003AE[], D_08B0040A[], D_08B0057E[], D_08B005D6[], D_08B00646[];
extern unsigned long long D_08B01290;
extern int func_08AF96B0(void);
extern void func_088D3CF0(void *, void *, void *);
extern unsigned long long func_089C6010(void);
extern unsigned long long func_0899924C(unsigned long long, unsigned long long);
extern void func_08AF12F8(Obj *);
extern void func_08AFB554(Obj *);
extern void func_08AF1434(Obj *);
extern void func_088D3F7C(int, void *, void *, int);
extern void func_088D428C(void *, void *, void *, int);
extern void func_08AF14F0(Obj *);
extern void func_08AFB8A8(Obj *);
extern void func_08AF7368(Obj *, int, int, int);
extern int func_08AF7338(Obj *);
int func_08AF19A8(Obj *self) {
    int c = func_08AF96B0();
    int one = 1;
    if (c) {
        func_088D3CF0(self->blk->a, self->blk->b, D_08B001D6);
    } else {
        func_088D3CF0(self->blk->a, self->blk->b, D_08B00176);
    }
    func_088D3CF0(self->blk->a, self->blk->b, D_08B00366);
    func_088D3CF0(self->blk->a, self->blk->b, D_08B003AE);
    H::put(self, (int)func_0899924C(func_089C6010(), D_08B01290));
    func_08AF12F8(self);
    func_08AFB554(self);
    func_08AF1434(self);
    func_088D3F7C(self->blk->h, self->blk->c, D_08B0040A, 3);
    func_088D428C(self->blk->b, self->blk->c, D_08B0057E, 1);
    if (func_08AF96B0()) {
        func_08AF14F0(self);
        func_088D428C(self->blk->b, self->blk->c, D_08B00646, 1);
    } else {
        func_088D428C(self->blk->b, self->blk->c, D_08B005D6, 1);
    }
    self->blk->k0 = 0xFFFF;
    self->blk->k1 = 0xFFFF;
    self->blk->k2 = 0xFFFF;
    self->blk->k3 = 0xFFFF;
    func_08AFB8A8(self);
    func_08AF7368(self, 0x10, 0, 1);
    if (H::on(D_089D7F90)) {
        self->blk->flags |= 0x20;
    } else {
        if (func_08AF7338(self)) func_08AF7368(self, 8, 0, 0);
    }
    self->blk->s = 5;
    return one;
}
}
