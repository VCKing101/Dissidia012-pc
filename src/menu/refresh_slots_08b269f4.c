typedef struct Pr { short s; unsigned short u; } Pr;
typedef struct Blk {
    char pad[0x194]; int w194[4]; char pad1A4[0x28C - 0x1A4]; int w28C[4]; Pr t29C[4]; Pr t2AC[4];
} Blk;
typedef struct Wk { Blk *b; } Wk;
extern void func_08B25AC8(Wk *, int);
extern void func_08B268FC(Wk *, int, int);
extern void func_08B26BC8(Wk *, int, int);
extern void func_08B27A98(Wk *, int, int);
extern void func_08B26E3C(Wk *, int, int);
extern void func_0887B944(int);
extern void func_08B25E3C(Wk *, unsigned char, int);
void func_08B269F4(Wk *self, unsigned char mode, int which)
{
    int i;
    int cell[1];
    int pad0[1];
    int v;
    unsigned int flags;
    if (mode == 1) {
        for (i = 0; i < 4; i++) {
            func_08B25AC8(self, i + 0x2A);
            if (which == 0) {
                v = self->b->t2AC[i].s;
                flags = self->b->t2AC[i].u;
            } else {
                v = self->b->t29C[i].s;
                flags = self->b->t29C[i].u;
            }
            if (v != -1) {
                func_08B268FC(self, i + 0x2A, v);
            }
            cell[0] = 0;
            if (v != -1 && (flags & 0x8000) != 0) {
                cell[0] = 1;
            }
            func_08B26BC8(self, i, cell[0]);
            self->b->w28C[i] = 3;
            if (cell[0] == 1) {
                self->b->w28C[i] = 0;
            }
        }
        for (i = 0; i < 4; i++) {
            func_08B27A98(self, i + 0x2A, -1);
        }
        for (i = 0; i < 4; i++) {
            func_08B26E3C(self, which, i);
        }
    } else {
        for (i = 0; i < 4; i++) {
            func_0887B944(self->b->w194[i]);
        }
        for (i = 0; i < 4; i++) {
            func_08B26E3C(self, 2, i);
        }
    }
    func_08B25E3C(self, mode, which);
}
