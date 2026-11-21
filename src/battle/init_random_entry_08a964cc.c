/* Starts an entry with two non-null handles and a random seed; -1 bad argument, -2 rejected. */
typedef struct Rnd38 { unsigned char active; char pad[3]; int a; int b; char pad2[0x64]; int seed; } Rnd38;
extern void func_08A9649C(Rnd38 *self);
extern int func_08AB3818(void *p);
extern int D_09AF84D0;
int func_08A964CC(Rnd38 *self, int a, int b)
{
    func_08A9649C(self);
    if (a == 0 || b == 0) {
        return -1;
    }
    if (func_08AB3818((char *)self + 0xC) != 0) {
        return -2;
    }
    self->seed = (D_09AF84D0 = D_09AF84D0 * 0x343FD + 0x269EC3) >> 16 & 0x7FFF;
    self->a = a;
    self->b = b;
    self->active = 1;
    return 0;
}
