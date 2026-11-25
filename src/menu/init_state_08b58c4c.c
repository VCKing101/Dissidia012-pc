typedef struct B64 { int v[16]; } B64;
typedef struct St {
    int f0; int f4; int f8; int fC;
    B64 f10;
    B64 f50;
    int f90; int f94; int f98; int f9C; int fA0; int fA4;
    char padA8[0xCE - 0xA8]; unsigned char fCE;
} St;
typedef struct Wk { St *s; } Wk;
extern int *D_089D7280;
extern int D_08B59210[];
extern void func_08935828(void *, int, int);
extern void func_088B168C(int *, B64 *, int);
extern void func_088729C0(void);
extern void func_088B16FC(int *);
extern int func_08B58B90(Wk *);
int func_08B58C4C(Wk *self, int a, int b, unsigned char mode)
{
    St *p;
    int one;
    func_08935828(&self->s->f10, 0, 0x40);
    p = self->s;
    func_088B168C(D_089D7280, &p->f10, 0x40);
    self->s->f4 = a;
    self->s->f8 = b;
    self->s->fC = 0;
    one = 1;
    *(int *)((char *)self->s + 0x40) = one;
    *(int **)((char *)self->s + 0x44) = (int *)((char *)self->s + 0x90);
    self->s->f90 = one;
    self->s->f98 = 0;
    self->s->fA0 = D_08B59210[self->s->f4];
    self->s->fA4 = (unsigned char)1;
    p = self->s;
    p->f50 = p->f10;
    self->s->fCE = one;
    func_088729C0();
    func_088B16FC(D_089D7280);
    if (mode != 0)
        return func_08B58B90(self);
    return one;
}
