/* Menu page enter: builds the page block and picks one of two layouts. */
typedef struct { char p[0x130]; int f130; char q[0x148 - 0x134]; int f148; } T;
typedef struct {
    int f0; T *f4; char p0[4]; char a0C[1]; char p1[0x58 - 0x0D]; int f58;
    char p2[0xE4 - 0x5C]; char aE4[1]; char p3[0xC00 - 0xE5]; unsigned int fC00;
} B;
typedef struct { char p[0xC18]; B *b; } S;
extern char D_08ACA332[], D_08ACA35E[], D_08ACA3CE[], D_08ACA38E[], D_08ACA3DE[];
extern void func_0887BA5C(void);
extern void func_088D3B88(int, T *, void *, int, int);
extern void func_088D3CF0(int, T *, void *);
extern void func_088D428C(T *, void *, void *, int);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_088D9CFC(int);
extern int func_088F14BC(void);
extern int func_088F1540(void);
extern void func_08AC7700(S *, void *);
extern void func_08AC8068(S *);
extern void func_08AC818C(S *);
extern void func_08AC82B8(S *);
extern int func_08AC8A94(S *, int, int, int);
void func_08AC7DAC(S *self) {
    T *t;
    func_08AC7700(self, (char *)self->b + 0xBC0);
    t = self->b->f4;
    func_088D3B88(self->b->f0, t, D_08ACA332, 0, 0x120);
    func_088D3CF0(self->b->f0, t, D_08ACA35E);
    if (self->b->f58 != 0) {
        func_0887BA5C();
        self->b->f58 = 0;
    }
    self->b->f58 = func_08AC8A94(self, 1, 0, 0);
    func_088D428C(t, self->b->a0C, D_08ACA3CE, 1);
    if (func_088F14BC() != 0) {
        func_088D9CFC(t->f148);
        func_08AC818C(self);
        func_088D3CF0(self->b->f0, t, D_08ACA38E);
        func_088D428C(t, self->b->a0C, D_08ACA3DE, 1);
        self->b->fC00 |= 0x14;
    } else {
        void *e;
        func_088D9CFC(t->f130);
        e = self->b->aE4;
        func_088D46E4(e, 0, 0, func_088F1540() - 1, 0, -1);
        func_08AC82B8(self);
        func_08AC8068(self);
        self->b->fC00 |= 0x54;
    }
}
