/* Menu page init: reads the two saved selections, clamps the second, and refreshes the lists for both. */
typedef struct {
    char p0[0x1C0]; int f1C0; int f1C4; char p1[0x1D4 - 0x1C8]; int f1D4; int f1D8; int f1DC;
} B;
typedef struct { B *b; } S;
typedef struct { char p[0x140]; } G0;
typedef struct { char p[0x140]; struct { int f0; char p[0x2BFC - 4]; int f2BFC; char q[0x2C04 - 0x2C00]; int f2C04; } *g; } G;
typedef struct { char p[1]; unsigned char b1; } R;
extern G *D_08B01CF8;
extern char *D_089D712C;
extern R *func_088CB164(char *, int);
extern void func_08B253E4(S *);
extern void func_08B259B8(S *, int, int, int);
extern void func_08B260A0(S *, int, int);
extern void func_08B26344(S *, int, int, int);
extern void func_08B266D4(S *, int, int);
extern void func_08B26CE4(S *, int);
extern void func_08B27D34(S *, int);
void func_08B25244(S *self) {
    int v[4];
    func_08B259B8(self, 0, -1, 0);
    self->b->f1C0 = D_08B01CF8->g->f0;
    self->b->f1C4 = D_08B01CF8->g->f2BFC;
    if (self->b->f1C4 <= 0) {
        self->b->f1C4 = 0;
    }
    func_08B26344(self, 1, self->b->f1C0, 0);
    func_08B26344(self, 1, self->b->f1C4, 1);
    func_08B27D34(self, 0);
    v[0] = func_088CB164(D_089D712C + 0xF58, self->b->f1C0)->b1;
    self->b->f1D4 = v[0];
    v[1] = v[0];
    v[2] = D_08B01CF8->g->f2C04;
    self->b->f1DC = v[2];
    if (v[2] > 0) {
        v[0] = v[2];
    }
    self->b->f1D8 = v[0];
    func_08B260A0(self, v[1], 1);
    func_08B26CE4(self, v[1]);
    func_08B266D4(self, v[0], v[1]);
    func_08B253E4(self);
}
