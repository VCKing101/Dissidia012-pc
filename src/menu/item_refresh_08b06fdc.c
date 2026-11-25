/* Menu item refresh: when the slot id is valid and the check passes, rebuilds the layout lists, reads the colour and draws the item. */
typedef struct Sl { char p[0xC]; short id; } Sl;
typedef struct W {
    char p0[4];
    char a4[0x400];
    char a404[0x284];
    Sl *sl; int pad; int h690;
    char p2[0x1F0];
    char a884[4];
} W;
typedef struct Col { unsigned char r, g, b, a; } Col;
typedef struct O { char p[0x138]; W *w; } O;
extern int func_08B06BA0(O *);
extern void func_088D3F00(void *, void *, void *);
extern void func_088D44EC(void *, void *, void *);
extern void func_088DA0A8(int, int, void *, void *);
extern void func_08B06E8C(O *, void *, int, int, unsigned char);
extern void func_08B071A0(O *);
extern char D_08B1C1E8[], D_08B1C288[], D_08B1C268[];
void func_08B06FDC(O *self) {
    char m[0x40];
    Col c;
    if (self->w->sl->id != -1) {
        if (func_08B06BA0(self)) {
            func_088D3F00(self->w->a4, self->w->a404, D_08B1C1E8);
            func_088D44EC(self->w->a404, self->w->a884, D_08B1C288);
            func_088D44EC(self->w->a404, self->w->a884, D_08B1C268);
            func_088DA0A8(self->w->h690, 0, m, &c);
            func_08B06E8C(self, (char *)self->w + 0x1F464, 0xCC, 0xE2, c.a);
            func_08B071A0(self);
        }
    }
}
