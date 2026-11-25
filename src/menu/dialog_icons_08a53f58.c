typedef struct B { unsigned char id; unsigned char k; } B;
typedef struct O { char p0[8]; int h8; int hc; int h10; int h14; int h18; int h1c; char p1[0x74-0x20]; B *b; int *tbl; } O;
extern char D_08A54638[];
extern void func_088D9F5C(int, int, int);
extern void func_088D9CFC(int);
extern void func_088D9CD8(int);
extern void func_088DA060(int);
extern int func_088DAA40(int, int, int);
extern void func_088DA0A8(int, int, void *, void *);
extern void func_088D9B84(int, void *);
extern void func_088D9AF4(int, void *);
extern void func_08935524(void *, void *, int);
void func_08A53F58(O *self) {
    char b140[64];
    char b100[16];
    char f0[64];
    char b0[4];
    int ai[36];
    int id = self->b->id;
    if (id == 0xff) {
        func_088D9F5C(self->h8, 0x14, 1);
        func_088D9CFC(self->hc);
        func_088D9CFC(self->h10);
        func_088D9CFC(self->h18);
        func_088D9F5C(self->h1c, 1, 1);
        if (self->h18 != 0) {
            func_088DA060(self->h18);
            self->h18 = 0;
        }
        self->h18 = func_088DAA40(self->tbl[0x8c / 4], 0, 0);
        func_088DA0A8(self->h14, 1, b140, b100);
        func_088D9B84(self->h18, b140);
        func_088D9AF4(self->h18, b100);
    } else if (0 < id && id < 0x23) {
        int idx;
        func_088D9F5C(self->h1c, 0, 1);
        func_08935524(ai, D_08A54638, 0x90);
        idx = ai[id];
        if (self->h18 != 0) {
            func_088DA060(self->h18);
            self->h18 = 0;
        }
        self->h18 = func_088DAA40(self->tbl[idx], 0, 0);
        func_088DA0A8(self->h14, 1, f0, b0);
        func_088D9B84(self->h18, f0);
        func_088D9AF4(self->h18, b0);
        if (self->b->k == 5) {
            func_088D9F5C(self->h8, 0x14, 1);
            func_088D9CFC(self->hc);
            func_088D9CFC(self->h10);
        } else {
            func_088D9F5C(self->h8, 0x13, 1);
            func_088D9CD8(self->hc);
            func_088D9CD8(self->h10);
        }
    } else {
        func_088D9F5C(self->h8, 0x14, 1);
        func_088D9CFC(self->hc);
        func_088D9CFC(self->h10);
        func_088D9CFC(self->h18);
        func_088D9CFC(self->h1c);
    }
}
