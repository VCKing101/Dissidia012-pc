/* Stores *value into the halfword at 0x09B6DB5E; when flag is set calls func_08B3C488(self, 3). */
struct Blk { char pad[0x9E]; short h; };
extern struct Blk D_09B6DAC0;
extern void func_08B3C488(void *self, int n);
void func_08B39000(void *self, int *value, unsigned char flag)
{
    D_09B6DAC0.h = *value;
    if (flag) { func_08B3C488(self, 3); }
}
