/* Constructor of a stage view: clears its link words, registers the draw callbacks, builds the child object and starts the stage job. The volatile byte store keeps it out of the jal delay slot. */
typedef struct S { int a; int b; void *child; char p0[0x120 - 0xC]; char sub[1]; } S;
extern int D_089D5EC0;
extern int D_089D6D78;
extern int func_08866F34(int v);
extern void func_08974880(void (*a)(void), void (*b)(void), int c, int d);
extern void func_0897497C(void *p);
extern void *func_08941350(int size);
extern void func_089405B0(void *p);
extern void func_08941388(void *p, int *args);
extern void func_088A4C4C(int v);
extern void func_088A57E4(int v);
extern void func_088A5270(void);
extern void func_088A52A4(void);
void func_088A5D78(S *self, int a, int b, int c, int d, int e)
{
    int args[4];
    void *p;
    void *r;
    self->b = 0;
    self->a = 0;
    args[0] = func_08866F34(D_089D5EC0);
    args[1] = c;
    args[2] = d;
    func_08974880(func_088A5270, func_088A52A4, a, b);
    func_0897497C(self->sub + 0x120 - 0x120 + 0);
    int z0 = 0;
    args[3] = z0;
    p = func_08941350(0x48);
    r = (void *)args[3];
    if (p != 0) {
        func_089405B0(p);
        r = p;
    }
    self->child = r;
    func_08941388(r, args);
    {
        volatile char *ch = (volatile char *)self->child; char z1 = 0;
        ch[0x34] = z1;
    }
    func_088A4C4C(e);
    func_088A57E4(D_089D6D78);
}
