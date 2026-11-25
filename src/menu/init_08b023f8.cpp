extern "C" {
typedef struct { char p0[0x3C]; int f3c; char p1[0x41C - 0x40]; int f41c; int f420; char p2[0x548 - 0x424]; int f548; char p3[0x648 - 0x54C]; unsigned flags; char p4[0x654 - 0x64C]; int f654; char p5[0x660 - 0x658]; short s[10]; int *p674; } Blk;
typedef struct { char p0[0x67C]; Blk *blk; } Obj;
typedef struct { char p0[0x140]; struct { char q0[0xAF4]; int f_af4; char q1[0x2E94 - 0xAF8]; int f_2e94; } *x; } Host;
extern Host *D_08B01CF8;
extern int *func_08AF34A0(Host *);
extern void func_08B0334C(Obj *);
extern void func_08B03FF8(Obj *, int, int, int, int);
extern void func_08B03080(Obj *, int, int, int);
extern void func_08B02D34(Obj *, int, int, int);
extern int func_08AF9F68(Host *);
extern int func_08AF9F48(Host *);
extern int func_08AF9F94(Host *);
extern int func_08AF9FB4(Host *);
extern void func_088D9CFC(int);
extern void func_08AF6D3C(Host *, int, int, int);
extern void func_08AF6E10(Host *, int);
extern void func_08AF7104(Host *);
extern void func_08AF3638(Host *, int);
struct P { static void go(int a, int b) { func_08AF6D3C(D_08B01CF8, a, b, 0); } };
void func_08B023F8(Obj *self) {
    int *a;
    int *b;
    int cell[2];
    int t;
    int i, k;
    a = func_08AF34A0(D_08B01CF8);
    self->blk->p674 = func_08AF34A0(D_08B01CF8);
    for (i = 0, k = 0, b = a; i < 5; i++, k += 4, b++) {
        int v = b[1];
        if (v > 0) *(short *)((char *)self->blk + 0x660 + k) = v;
    }
    func_08B0334C(self);
    for (i = 0, k = 0; i < 5; i++, k += 4) {
        int v = ((short *)((char *)self->blk + k))[0x660 / 2];
        if (v != 0) func_08B03FF8(self, 1, v, i, 0);
    }
    func_08B03080(self, 1, 0, 0);
    cell[0] = D_08B01CF8->x->f_af4;
    func_08B02D34(self, 2, cell[0], 0);
    if (func_08AF9F68(D_08B01CF8) == 0) {
        cell[0] = D_08B01CF8->x->f_2e94;
        func_08B02D34(self, 3, cell[0], 0);
        t = self->blk->f420;
    } else {
        func_088D9CFC(self->blk->f41c);
        t = self->blk->f420;
    }
    func_088D9CFC(t);
    P::go(self->blk->f3c, self->blk->f548);
    func_08AF6E10(D_08B01CF8, 0x2D);
    if (func_08AF9F48(D_08B01CF8) == 1 || func_08AF9F68(D_08B01CF8) == 1 || func_08AF9F94(D_08B01CF8) == 1 || func_08AF9FB4(D_08B01CF8) == 1) {
        self->blk->flags |= 0x200;
    }
    if (func_08AF9FB4(D_08B01CF8) == 1) {
        self->blk->flags |= 0x100;
    }
    self->blk->f654 = *self->blk->p674;
    if ((self->blk->flags & 0x200) != 0) func_08AF7104(D_08B01CF8);
    func_08AF3638(D_08B01CF8, 0xC6D);
}
}
