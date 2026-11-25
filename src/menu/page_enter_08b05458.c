/* Menu page enter: loads the layout lists (two variants), then refreshes the nine rows. */
typedef struct {
    char p0[4]; char a4[1];
    char p1[0x404 - 5]; char a404[1];
    char p2[0x4F4 - 0x405]; int f4F4;
    char p3[0x884 - 0x4F8]; char a884[1];
} B;
typedef struct { char p[0x138]; B *b; } S;
extern int D_08B01CF8;
extern char D_08B1BC1C[], D_08B1BC4C[], D_08B1BBF4[], D_08B1BCBC[], D_08B1BE1A[];
extern void func_088D3EA0(void *, void *, void *);
extern void func_088D436C(void *, void *, void *);
extern void func_088D9F5C(int, int, int);
extern void func_08AF5030(int, int);
extern void func_08B05254(S *);
extern void func_08B0557C(S *);
extern void func_08B080BC(S *);
extern int func_08B0DF10(S *);
extern void func_08B106F8(S *, int);
void func_08B05458(S *self) {
    int i;
    char *p, *q;
    int r = func_08B0DF10(self);
    p = self->b->a4;
    q = self->b->a404;
    if (r != 0) {
        func_088D3EA0(p, q, D_08B1BC1C);
        func_088D436C(self->b->a404, self->b->a884, D_08B1BC4C);
        p = self->b->a4;
        q = self->b->a404;
    } else {
        func_088D3EA0(p, q, D_08B1BBF4);
        p = self->b->a4;
        q = self->b->a404;
    }
    func_088D3EA0(p, q, D_08B1BCBC);
    func_088D3EA0(self->b->a4, self->b->a404, D_08B1BE1A);
    func_08B05254(self);
    func_088D9F5C(self->b->f4F4, 0x23, 1);
    for (i = 0; i < 9; i++) {
        func_08B106F8(self, i);
    }
    if (func_08B0DF10(self) == 0) {
        func_08AF5030(D_08B01CF8, 0);
    }
    func_08B0557C(self);
    func_08B080BC(self);
}
