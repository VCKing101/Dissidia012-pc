typedef struct Wk {
    char pad0[0x488]; char f488[1]; char pad489[0x6D4 - 0x489]; int f6D4;
    char pad6D8[0xA8C - 0x6D8]; int fA8C[1]; char padA90[0xEF8 - 0xA90]; char fEF8[0x80];
    char padF78[0x2734 - 0xF78]; unsigned short f2734; char pad2736[0x3210 - 0x2736]; char f3210[4]; int f3214;
} Wk;
typedef struct Sc { char pad[0x14C]; Wk *wk; } Sc;
typedef struct Pk { char pad[4]; unsigned short idx; } Pk;
extern Pk D_08A0A672;
extern int D_089D6998;
extern char D_08A0AC54[], D_08A09B0A[];
extern int func_089F2758(Sc *, char *);
extern int func_089F26E0(Sc *, char *);
extern void func_0887BA5C(int);
extern void func_0893476C(char *, char *, int);
extern void func_0887DF14(int, char *, int, char *);
extern int func_0887D868(int, int, char *, int, int, int, int, int);
extern void func_088D428C(char *, int *, char *, int);
extern void func_088D44EC(char *, int *, char *);
extern void func_088D9F5C(int, int, int);
extern void func_089F5AD4(Sc *);
extern void func_089F80F8(Sc *);
void func_089F3498(Sc *self, unsigned char mode)
{
    char buf[64];
    int r;
    Wk *w;
    r = func_089F2758(self, self->wk->f3210);
    w = self->wk;
    if (r != 0) {
        int n;
        int k;
        n = func_089F26E0(self, w->f3210);
        n = self->wk->f2734 + n;
        if (self->wk->fA8C[D_08A0A672.idx] != 0) {
            func_0887BA5C(self->wk->fA8C[D_08A0A672.idx]);
            self->wk->fA8C[D_08A0A672.idx] = 0;
        }
        func_0893476C(buf, D_08A0AC54, n);
        func_0887DF14(D_089D6998, self->wk->fEF8, 0x80, buf);
        {
            char *lab = self->wk->fEF8;
            self->wk->fA8C[D_08A0A672.idx] = func_0887D868(D_089D6998, 0, lab, -1, 0, 0, 0, 0);
        }
        func_088D428C(self->wk->f488, self->wk->fA8C, D_08A09B0A, 1);
        k = func_089F2758(self, self->wk->f3210);
        w = self->wk;
        if (k == 0) {
            w->f2734 = n;
            w = self->wk;
            if (mode == 0) {
                if (w->f3214 > 0) {
                    func_088D9F5C(w->f6D4, 0x38, 1);
                    *(short *)(self->wk->f6D4 + 0xE) = 0x39;
                }
                func_089F5AD4(self);
                func_089F80F8(self);
                w = self->wk;
            }
        }
    }
    func_088D44EC(w->f488, w->fA8C, D_08A09B0A);
}
