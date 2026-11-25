/* Loads the stage's data file into a temporary 1 MB block, hands it to the parser and frees the block. */
typedef struct S { char p0[8]; int heap; } S;
extern void *D_089D6AC8;
extern char D_089CE140[];
extern int func_0885C6CC(int heap, int size, int align, int a, int b, int c);
extern char *func_0889381C(void *p);
extern void func_0893476C(char *buf, char *fmt, char *name);
extern int func_08862854(char *path, int blk, int size, int flag);
extern void func_08910B54(S *self, int blk, int size, int r);
extern void func_0885C538(int heap, int blk, int a);
void func_08910A9C(S *self)
{
    char path[0x100];
    int blk;
    unsigned size = 0x100000;
    int one = 1;
    blk = func_0885C6CC(self->heap, 0x100000, 0x40, 0, one, 1);
    func_0893476C(path, D_089CE140, func_0889381C(D_089D6AC8));
    func_08910B54(self, blk, size, func_08862854(path, blk, size, 1));
    func_0885C538(self->heap, blk, 0x40);
}
