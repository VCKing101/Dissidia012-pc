/* Formats "talkevent %s" with the name at 0x09B6DB10 into a 256-byte buffer, calls func_088E9958(0x09B6DB50 word), func_088E980C(buf, 1), func_088E991C(0, func_08B3CC6C). */
extern char D_08B418AC[];
extern int D_09B6DAC0[];
extern void func_0893476C(char *buf, char *fmt, char *name);
extern void func_088E9958(int v);
extern void func_088E980C(char *buf, int one);
extern void func_088E991C(int z, void *fn);
extern void func_08B3CC6C(void);
void func_08B3D690(void)
{
    char buf[0x100];
    int *blk = D_09B6DAC0;
    func_0893476C(buf, D_08B418AC, (char *)blk + 0x50);
    func_088E9958(blk[0x90 / 4]);
    func_088E980C(buf, 1);
    func_088E991C(0, func_08B3CC6C);
}
