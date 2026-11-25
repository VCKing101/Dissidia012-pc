/* Creates an 8-byte pool from func_08B42B40(), stores it at self->0 and stores D_089D8518 + 0x240 at pool+4. */
struct Pool { int a; int b; };
extern char *D_089D8518;
extern int func_08B42B40(void);
extern struct Pool *func_0885C6CC(int a, int size, int b, int c, int d, int e);
void func_08B49754(struct Pool **self)
{
    int r = func_08B42B40();
    int one = 1;
    struct Pool *p = func_0885C6CC(r, 8, 0, 0, one, 0);
    *self = p;
    p->b = (int)(D_089D8518 + 0x240);
}
