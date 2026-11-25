/* Loads the play-plan archive for the chocobo scene: opens the archive, resolves its sequence, gim and se entries, creates the scene model and its sound, and flags the scene ready. */
typedef struct Ent2718 { int p0; int a; int b; } Ent2718;
typedef struct Inner2718 {
    int h0;
    char pad4[0x3FC];
    int h400;
    char pad404[0x47C];
    int f880;
    int f884;
    int f888;
    int f88C;
    unsigned short s890;
    unsigned char c892;
} Inner2718;
extern int D_089D7830;
extern int D_089D6C80;
extern char *D_08B12DE4;
extern char *D_08B12DE8;
extern char *D_08B12DEC;
extern char *D_08B12DF0;
extern char *D_08B12DF4;
extern char D_08B12DB4[];
extern int func_088FEA4C(void);
extern void func_08873DEC(int, int);
extern int func_08862C40(char *, int);
extern int func_0885C6CC(int, int, int, int, int, int);
extern void func_08862854(char *, int, int, int);
extern void func_08869954(int *, int);
extern Ent2718 *func_08869AB4(int *, char *);
extern void func_08B125D8(void *);
extern void func_08B12630(void *);
extern int func_088CF188(int, int, int, int, int, int, int);
extern int func_088A0DBC(int, char *, int, int, int);
extern int func_088DAA40(int, int, int);
typedef struct Self2718 { int p0; int h4; Inner2718 *o8; } Self2718;

void func_08B12718(Self2718 *self){ int r = func_088FEA4C(); if (r != 0) { return; } else { if (self->o8->c892 != 0) {
            int u;
            Ent2718 *a, *b, *c;
            func_08873DEC(0, 2);
            u = func_08862C40(D_08B12DE4, 1); { int one = 1; self->o8->f884 = func_0885C6CC(self->h4, u, 0, 0, one, 0); }
            func_08862854(D_08B12DE4, self->o8->f884, u, 1);
            func_08869954(&self->o8->f880, self->o8->f884);
            a = func_08869AB4(&self->o8->f880, D_08B12DEC);
            b = func_08869AB4(&self->o8->f880, D_08B12DF0);
            c = func_08869AB4(&self->o8->f880, D_08B12DE8);
            func_08B125D8(self);
            self->o8->h0 = func_088CF188(D_089D7830, a->a, a->b, b->a, b->b, c->a, c->b);
            func_08B12630(self);
            a = func_08869AB4(&self->o8->f880, D_08B12DF4);
            self->o8->f888 = func_088A0DBC(D_089D6C80, D_08B12DB4, a->a, a->b, 0);
            self->o8->h400 = func_088DAA40(self->o8->h0, self->o8->s890, 0);
 } self->o8->f88C = 1; } }