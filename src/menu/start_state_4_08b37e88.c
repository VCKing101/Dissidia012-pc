/* After func_08B39F78(self), stores *src (two words) at 0x09B6DB80 and a value at 0x09B6DB74, sets state 4 at self+0x94 and calls func_08B3CAF0(0, func_08B3D42C). */
struct Blk { char pad[0xB4]; int h; char pad2[0xC0 - 0xB8]; int a, b; };
struct Self { char pad[0x94]; int state; };
struct Pair { int a, b; };
extern struct Blk D_09B6DAC0;
extern void func_08B39F78(struct Self *self);
extern void func_08B3CAF0(int z, void *fn);
extern void func_08B3D42C(void);
void func_08B37E88(struct Self *self, struct Pair *src, int h)
{
    func_08B39F78(self);
    int a = src->a;
    int b = src->b;
    D_09B6DAC0.a = a;
    D_09B6DAC0.b = b;
    D_09B6DAC0.h = h;
    self->state = 4;
    func_08B3CAF0(0, func_08B3D42C);
}
