/* Menu page init: runs the setup calls, then picks the 0x80 branch (state 1 or 3). */
typedef struct { char p[0x10]; int h10; } I;
typedef struct { int f0; char p[0xC9]; char b_cd; } C;
typedef struct { char p[0x44]; unsigned int flags; char q[0xC4 - 0x48]; C *c; } S;
extern int D_089D7ED0, D_089D4960;
extern void func_088EBCCC(int, int, int);
extern void func_088EBEBC(int);
extern void func_08844A60(int);
extern int func_08920B18(int, int);
extern void func_08B34C84(S *);
extern void func_08B34F80(S *);
extern void func_08B35030(S *);
extern void func_08B39C20(S *);
extern void func_08B3B6F0(S *);
extern void func_08B3B764(S *, int);
extern void func_08B3BCA4(S *);
extern void func_08B3C14C(S *);
extern void func_08B3C554(S *, int, int);
extern int func_08B42B40(void);
extern int func_08B45EEC(int);
extern int func_08B45EF8(int);
extern int func_08B46DDC(int);
extern int func_08B46DE8(int);
extern void func_08B488F4(int, int, int);
void func_08B364AC(S *self) {
    int a;
    int go;
    func_088EBCCC(D_089D7ED0, 1, 0);
    func_08B34C84(self);
    a = func_08B45EF8(*(int *)((char *)self->c + 0x10));
    func_08920B18(a, func_08B42B40());
    a = func_08B45EEC(*(int *)((char *)self->c + 0x10));
    func_08B488F4(a, func_08B42B40(), 0);
    a = func_08B46DDC(*(int *)((char *)self->c + 0x10));
    func_08B3C554(self, a, func_08B46DE8(*(int *)((char *)self->c + 0x10)));
    func_08844A60(D_089D4960);
    func_08B34F80(self);
    self->c->b_cd = 0;
    func_088EBEBC(D_089D7ED0);
    go = (self->flags & 0x80) != 0;
    go = (unsigned char)go;
    if (go) {
        self->flags &= ~0x80;
        func_08B3B6F0(self);
        func_08B39C20(self);
        self->c->f0 = 1;
    } else {
        func_08B35030(self);
        func_08B3B764(self, 1);
        func_08B3BCA4(self);
        self->c->f0 = 3;
    }
    func_08B3C14C(self);
}
