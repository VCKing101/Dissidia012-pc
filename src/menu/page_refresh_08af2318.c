/* Menu page step: refreshes the icon animation, fades from white when the fade value is set, then runs the shared update calls and flags the page. */
typedef struct W {
    char p0[0x41C]; int anim;
    char p1[0x720]; unsigned fB40;
    char p2[0xC4]; int fC08;
    char p3[0x2104]; float f2D10;
    char p4[0xC]; char f2D20;
    char p5[0xE9]; unsigned short f2E0A;
} W;
typedef struct O { char p[0x140]; W *w; } O;
extern void func_088D9F5C(int anim, int id, int one);
extern void func_08AF08CC(void *);
extern void func_08873D34(float, int, void (*)(void), int, int, int);
extern void func_08873DB0(float *, int);
extern int func_088D4C5C(void);
extern void func_088853CC(int, int);
extern void func_08AF70A4(O *);
extern void func_08AF74CC(O *);
extern int D_089D69E8;
extern void func_08AF7574(void);
void func_08AF2318(O *self) {
    func_088D9F5C(self->w->anim, self->w->f2E0A, 1);
    if (self->w->fC08 != 0) {
        func_08AF08CC(&self->w->anim);
    }
    if (self->w->f2D10 != 0.0f) {
        float c[4];
        self->w->f2D20 = 0;
        func_08873D34((float)func_088D4C5C(), 3, func_08AF7574, 0, 1, 2);
        c[0] = 255.0f; c[1] = 255.0f; c[2] = 255.0f; c[3] = 255.0f;
        func_08873DB0(c, 2);
    }
    { int g = D_089D69E8; int zero = 0; func_088853CC(g, zero); }
    func_08AF70A4(self);
    func_08AF74CC(self);
    self->w->fB40 |= 1;
}
