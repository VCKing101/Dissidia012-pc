typedef struct S { char p[0x10]; int f904, f908, f90C, f910, f914, f918, f91C, f920, f924, f928, f92C, f930, f934, f938, f93C, f940; } S;
typedef struct W { char p0[0x8F4]; S s;
  char p1[0xA74 - 0x944]; int fA74; int fA78; int fA7C; int fA80; int fA84; char p2[0xB2A - 0xA88]; char cB2A; } W;
typedef struct Obj { char p[0x14C]; W *w; } Obj;
extern int D_089D6998;
extern unsigned char D_089D5F20;
extern unsigned short D_08AC42A2[], D_08AC42B6[], D_08AC42EE[], D_08AC42F6[];
extern int func_0887D868(int, int, void *, int, int, int, int, int);
extern int func_0887E534(int);
extern int func_08ABBC4C(Obj *, int, int, int, int);
extern int func_08ABC934(Obj *, int);

void func_08ABFC04(Obj *self)
{
    S *w = &self->w->s;
    char *name = &self->w->cB2A;
    int h;
    int r;
    if (D_089D5F20 != 0) {
        h = func_0887D868(D_089D6998, 0, name, -1, 0, 0, 0, 0);
        w->f904 = h;
        w->f908 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, D_08AC42EE[self->w->fA80 * 2], 0, 0);
        w->f90C = h;
        w->f910 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, D_08AC42F6[self->w->fA84 * 2], 0, 0);
        w->f914 = h;
        w->f918 = func_0887E534(h);
    } else {
        h = func_0887D868(D_089D6998, 0, name, -1, 0, 0, 0, 0);
        w->f904 = h;
        w->f908 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, D_08AC42A2[self->w->fA7C * 2], 0, 0);
        w->f90C = h;
        w->f910 = func_0887E534(h);
        r = func_08ABC934(self, self->w->fA74);
        h = func_08ABBC4C(self, 0, D_08AC42B6[r * 4 + 1], 0, 0);
        w->f914 = h;
        w->f918 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, D_08AC42EE[self->w->fA80 * 2], 0, 0);
        w->f91C = h;
        w->f920 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, D_08AC42F6[self->w->fA84 * 2], 0, 0);
        w->f924 = h;
        w->f928 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, 0x30, 0, 0);
        w->f92C = h;
        w->f930 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, 0x31, 0, 0);
        w->f934 = h;
        w->f938 = func_0887E534(h);
        h = func_08ABBC4C(self, 0, 0x32, 0, 0);
        w->f93C = h;
        w->f940 = func_0887E534(h);
    }
}
