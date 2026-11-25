typedef struct S {
    char p0[0x210]; int f210; char p214[0xC98]; int feac; char peb0[4]; int feb4; char peb8[0x40];
    unsigned fef8; unsigned char fefc;
} S;
extern void func_088AC1AC(S *, int, int);
extern void func_088AC294(S *, int);
extern int func_088ACEE0(S *, int, int);
extern unsigned D_089D712C;
extern int func_08920970(unsigned);
extern void func_089127C4(unsigned);
extern void func_08920978(unsigned);
extern void func_088ABD7C(S *);
extern void func_088AC7EC(S *);
extern void func_088AC7C4(S *);
extern void func_088ACDF8(S *, int, int, int);
extern void func_088B1764(int);
extern void func_08872A18(void);
extern int D_089D7280;

void func_088ACAFC(S *self, int arg)
{
    int done = 0;
    if (self->feac == 1) {
        int m = self->f210;
        if (m == 1) {
            func_088AC1AC(self, 5, self->fefc);
        } else if (m == 0) {
            func_088AC294(self, 4);
        } else if (m == 5) {
            func_088AC1AC(self, 5, self->fefc);
        } else {
            done = 1;
        }
        self->feac = 0;
    } else {
        int r = func_088ACEE0(self, self->feac, arg);
        if (r == -1) {
            if (self->feac == 0xE) {
                if (arg == 2) {
                    unsigned a, b;
                    int k;
                    self->fef8 = self->fef8 & 0xFFFFFBFF;
                    a = D_089D712C + 0x3F63C;
                    b = D_089D712C + 0x3E8C8;
                    k = func_08920970(a);
                    if (k >= 0) {
                        func_089127C4(b + k * 0x280);
                    }
                    func_08920978(a);
                } else {
                    func_088ABD7C(self);
                    self->fef8 = self->fef8 | 0x400;
                }
            }
            done = 1;
            self->feac = 0;
        } else {
            self->feac = r;
            if (r == 0xF) {
                func_088AC1AC(self, 5, self->fefc);
                self->feac = 0xC;
            } else if (r == 0x10 || r == 0x11) {
                int v;
                func_088AC7EC(self);
                v = r == 0x10 ? 0 : 0xC;
                self->feac = v;
            } else if (r == 0x12) {
                func_088AC7C4(self);
                self->feac = 8;
            } else {
                func_088ACDF8(self, self->feac, self->feb4, 0);
            }
        }
    }
    if (done) {
        unsigned f;
        if ((self->fef8 & 2) == 0) {
            func_088B1764(D_089D7280);
        }
        func_08872A18();
        f = self->fef8;
        if ((f & 4) != 0) {
            f = f & 0xFFFFFFFB;
            self->fef8 = f;
        }
        if ((f & 1) != 0) {
            f = f & 0xFFFFFDFE;
            self->fef8 = f;
        }
        if ((f & 0x8000) != 0) {
            self->fef8 = f & 0xFFFF7DFF;
        }
    }
}
