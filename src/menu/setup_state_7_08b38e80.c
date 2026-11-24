/* Stores a at 0x09B6DB50, calls func_089359A4(0x09B6DB10, c), sets self+0x94 to 7 and calls func_08B3CAF0(0, func_08B3D690). */
struct Self { char pad[0x94]; int state; };
extern int D_09B6DAC0[];
extern void func_089359A4(char *dst, int c);
extern void func_08B3CAF0(int a, void *fn);
extern void func_08B3D690(void);
void func_08B38E80(struct Self *self, int a, int c)
{
    D_09B6DAC0[0x90 / 4] = a;
    func_089359A4((char *)D_09B6DAC0 + 0x50, c);
    self->state = 7;
    func_08B3CAF0(0, func_08B3D690);
}
