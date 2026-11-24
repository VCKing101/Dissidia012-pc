/* After func_08B39F78(self), stores a value at 0x09B6DB74 and *src at 0x09B6DB78, sets state 5 at self+0x94 and calls func_08B3CAF0(0, func_08B41330). */
struct Blk { char pad[0xB4]; int h; int a; };
struct Self { char pad[0x94]; int state; };
extern struct Blk D_09B6DAC0;
extern void func_08B39F78(struct Self *self);
extern void func_08B3CAF0(int z, void *fn);
extern void func_08B41330(void);
void func_08B37EF8(struct Self *self, int *src, int h)
{
    func_08B39F78(self);
    D_09B6DAC0.h = h;
    D_09B6DAC0.a = *src;
    self->state = 5;
    func_08B3CAF0(0, func_08B41330);
}
