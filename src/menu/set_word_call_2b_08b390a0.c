/* Stores *value into the word at 0x09B6DACC; when flag is set calls func_08B3C488(self, 0x2B). */
struct Blk { char pad[0xC]; int w; };
extern struct Blk D_09B6DAC0;
extern void func_08B3C488(void *self, int n);
void func_08B390A0(void *self, int *value, unsigned char flag)
{
    D_09B6DAC0.w = *value;
    if (flag) { func_08B3C488(self, 0x2B); }
}
