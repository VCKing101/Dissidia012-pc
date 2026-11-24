/* Sets a byte flag in D_09B6DAC0, stores 12 at +0x94 and calls func_08B3CAF0 with a handler. */
extern char D_09B6DAC0[];
extern void func_08B3CAF0(int a, void (*fn)(void));
extern void func_08B3D760(void);
void func_08B39940(int *self)
{
    D_09B6DAC0[0x21D] = 1;
    self[0x94 / 4] = 12;
    func_08B3CAF0(0, func_08B3D760);
}
