/* If bit 0 of *flags is set, sets bit 0 of the halfword at 0x09B6DAEA; if bit 1, sets bit 0 of the halfword at 0x09B6DB04; when go is set calls func_08B3C488(self, 0x24). */
struct Blk { char pad[0x2A]; unsigned short a; char pad2[0x44 - 0x2C]; unsigned short b; };
extern struct Blk D_09B6DAC0;
extern void func_08B3C488(void *self, int n);
void func_08B39034(void *self, unsigned *flags, unsigned char go)
{
    if ((*flags & 1) != 0) { D_09B6DAC0.a = D_09B6DAC0.a | 1; }
    if ((*flags & 2) != 0) { D_09B6DAC0.b = D_09B6DAC0.b | 1; }
    if (go != 0) { func_08B3C488(self, 0x24); }
}
