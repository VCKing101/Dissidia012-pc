/* Sets byte 0x021F = 1 and 0x021E = 0 of 0x09B6DAC0, then runs func_08B3E004(t, 0), func_08B361D8(t), func_08B363E8(t, 1), func_0891FEC0(D_089D8528), func_08B3C900(5, -1, -1, -1). */
struct Blk { char pad[0x21E]; unsigned char a, b; };
extern struct Blk D_09B6DAC0;
extern void *D_08B41CC0;
extern void *D_089D8528;
extern void func_08B3E004(void *t, int z);
extern void func_08B361D8(void *t);
extern void func_08B363E8(void *t, int one);
extern void func_0891FEC0(void *o);
extern void func_08B3C900(int a, int b, int c, int d);
void func_08B3CBD8(void)
{
    void *t;
    D_09B6DAC0.b = 1;
    t = D_08B41CC0;
    D_09B6DAC0.a = 0;
    func_08B3E004(t, 0);
    func_08B361D8(t);
    func_08B363E8(t, 1);
    func_0891FEC0(D_089D8528);
    func_08B3C900(5, -1, -1, -1);
}
