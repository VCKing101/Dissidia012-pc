typedef struct M { char p0[0x400]; char b400[0x480]; char t880[0x9b8-0x880]; int h9b8; int h9bc; char p1[0x9d0-0x9c0]; int h9d0; int h9d4; char p2[0x1098-0x9d8]; int h1098; int h109c; char p3[0x1144-0x10a0]; unsigned char fl; } M;
typedef struct O { char p0[0x1320]; M *m; } O;
typedef struct MM { char p0[0x4b0]; int h4b0; char p1[0x59c-0x4b4]; int h59c; char p2[0x5a8-0x5a0]; int h5a8; } MM;
extern void *D_08A23FF0;
extern char D_08A11B2E[], D_08A11B3E[], D_08A11C2E[];
extern void func_0887BA5C(int);
extern int func_0887E534(int);
extern void func_088D3CF0(void *, void *, char *);
extern void func_088D428C(void *, void *, char *, int);
extern void func_088D46E4(void *, int, int, int, int, int);
extern void func_088D9F5C(int, int, int);
extern void func_088D9CFC(int);
extern int func_08A1D42C(void *, int, int, int, int);
extern void func_08A0CF5C(O *);
extern void func_08A0CD80(O *);
extern void func_08A0D0F0(O *);
void func_08A0EF84(O *self, unsigned char arg) {
    int v;
    int i;
    int one;
    if (self->m->h9bc != 0) {
        func_0887BA5C(self->m->h9bc);
        self->m->h9bc = 0;
    }
    if (self->m->h9d4 != 0) {
        func_0887BA5C(self->m->h9d4);
        self->m->h9d4 = 0;
    }
    v = (self->m->fl & 0x40) ? 0x34 : 0x33;
    self->m->h9bc = func_08A1D42C(D_08A23FF0, 0, v, 0, 0);
    self->m->h9d4 = func_0887E534(self->m->h9bc);
    if (self->m->h9b8 != 0) {
        func_0887BA5C(self->m->h9b8);
        self->m->h9b8 = 0;
    }
    if (self->m->h9d0 != 0) {
        func_0887BA5C(self->m->h9d0);
        self->m->h9d0 = 0;
    }
    i = 0x58;
    if (self->m->fl & 0x80) i = 0x59;
    self->m->h9b8 = func_0887E534(*(int *)((char *)self->m + i * 4 + 0x880));
    self->m->h9d0 = func_0887E534(*(int *)((char *)self->m + i * 4 + 0x880));
    func_088D3CF0(self->m, self->m->b400, D_08A11B2E);
    func_088D3CF0(self->m, self->m->b400, D_08A11B3E);
    func_088D428C(self->m->b400, self->m->t880, D_08A11C2E, 1);
    i = 0;
    one = 1;
    do {
        if (self->m->fl & (one << i)) {
            func_088D9F5C(((MM *)self->m)->h5a8, 0x49, 1);
        }
        i++;
    } while (i < 6);
    v = self->m->h109c;
    if (arg != 0) v = 0;
    func_088D46E4(&self->m->h1098, v, 0, 5, 0, -1);
    func_08A0CF5C(self);
    func_088D9F5C(((MM *)self->m)->h4b0, 8, 1);
    func_088D9CFC(((MM *)self->m)->h59c);
    func_08A0CD80(self);
    func_08A0D0F0(self);
}
