typedef struct { char p0[0xC]; short s; } Rec;
typedef struct {
    char p0[4]; char a[1]; char pa[0x404 - 5]; char b[1]; char pb[0x61C - 0x405]; Rec *r1; char p2[0x640 - 0x620]; Rec *r2;
    char p3[0x884 - 0x644]; char c[1]; char p4[0x8018 - 0x885]; char t0[0x30]; int c0; char t1[0x30]; int c1;
} Blk;
typedef struct { char p0[0x138]; Blk *blk; } Obj;
extern char D_08B22770[], D_08B22788[], D_08B227D8[], D_08B227F8[], D_08B22808[], D_08B22818[], D_08B22868[], D_08B22888[], D_08B22898[];
extern void func_08B1F5AC(Obj *);
extern void func_08B1FD1C(Obj *);
extern void func_08B1FFAC(Obj *, int, void *);
extern void func_088D3F00(void *, void *, void *);
extern void func_088D44EC(void *, void *, void *);
void func_08B2041C(Obj *self) {
    if (self->blk->c0 != 0) {
        self->blk->c0 = self->blk->c0 - 1;
        if (self->blk->c0 == 1) {
            func_08B1F5AC(self);
        } else if (self->blk->c0 <= 0) {
            func_08B1FFAC(self, 0, self->blk->t0);
            self->blk->c0 = 0;
        }
    }
    if (self->blk->c1 != 0) {
        self->blk->c1 = self->blk->c1 - 1;
        if (self->blk->c1 == 1) {
            func_08B1FD1C(self);
        } else if (self->blk->c1 <= 0) {
            func_08B1FFAC(self, 1, self->blk->t1);
            self->blk->c1 = 0;
        }
    }
    func_088D3F00(self->blk->a, self->blk->b, D_08B22770);
    if (self->blk->r1->s != -1) {
        func_088D3F00(self->blk->a, self->blk->b, D_08B22788);
        func_088D44EC(self->blk->b, self->blk->c, D_08B227D8);
        func_088D44EC(self->blk->b, self->blk->c, D_08B227F8);
        func_088D44EC(self->blk->b, self->blk->c, D_08B22808);
    }
    if (self->blk->r2->s != -1) {
        func_088D3F00(self->blk->a, self->blk->b, D_08B22818);
        func_088D44EC(self->blk->b, self->blk->c, D_08B22868);
        func_088D44EC(self->blk->b, self->blk->c, D_08B22888);
        func_088D44EC(self->blk->b, self->blk->c, D_08B22898);
    }
}
