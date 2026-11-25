typedef struct Rf { char pad[0x10]; unsigned int fl; } Rf;
typedef struct Sub {
    char pad[0x30]; int a30; int a34; int a38; int a3C; int a40; int a44; int a48;
} Sub;
typedef struct Q { char pad[0xBEC]; unsigned short sBEC[4]; } Q;
typedef struct Blk { char pad[0xC]; Sub s; } Blk;
typedef struct Wk { char pad[0xC18]; Blk *b; } Wk;
extern void func_0887BA5C(int);
extern int func_08AC77B4(Wk *, int, int, int);
extern Rf *func_0887E534(int);
void func_08AC82B8(Wk *self)
{
    Sub *b;
    Q *c;
    int h;
    Rf *r;
    b = &self->b->s;
    if (b->a30 != 0) {
        func_0887BA5C(b->a30);
        b->a30 = 0;
    }
    if (b->a34 != 0) {
        func_0887BA5C(b->a34);
        b->a34 = 0;
    }
    if (b->a40 != 0) {
        func_0887BA5C(b->a40);
        b->a40 = 0;
    }
    if (b->a38 != 0) {
        func_0887BA5C(b->a38);
        b->a38 = 0;
    }
    if (b->a44 != 0) {
        func_0887BA5C(b->a44);
        b->a44 = 0;
    }
    if (b->a3C != 0) {
        func_0887BA5C(b->a3C);
        b->a3C = 0;
    }
    if (b->a48 != 0) {
        func_0887BA5C(b->a48);
        b->a48 = 0;
    }
    c = (Q *)self->b;
    if (c->sBEC[0] != 0xFFFF) {
        b->a30 = func_08AC77B4(self, c->sBEC[0], 0, 0);
        c = (Q *)self->b;
    }
    if (c->sBEC[1] != 0xFFFF) {
        h = func_08AC77B4(self, c->sBEC[1], 0, 0);
        b->a34 = h;
        r = func_0887E534(h);
        b->a40 = (int)r;
        r->fl = r->fl | 0x800;
        c = (Q *)self->b;
    }
    if (c->sBEC[2] != 0xFFFF) {
        h = func_08AC77B4(self, c->sBEC[2], 0, 0);
        b->a38 = h;
        r = func_0887E534(h);
        b->a44 = (int)r;
        r->fl = r->fl | 0x800;
        c = (Q *)self->b;
    }
    if (c->sBEC[3] != 0xFFFF) {
        h = func_08AC77B4(self, c->sBEC[3], 0, 0);
        b->a3C = h;
        r = func_0887E534(h);
        b->a48 = (int)r;
        r->fl = r->fl | 0x800;
    }
}
