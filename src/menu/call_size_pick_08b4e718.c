/* Calls func_08932000(self+0x40, self->0x38, self->0x3C, flag ? 0xC : 0xF). */
struct Self { char pad[0x38]; int a; int b; };
extern void func_08932000(char *p, int a, int b, int c);
void func_08B4E718(struct Self *self, unsigned char flag)
{
    int n = flag ? 0xC : 0xF;
    func_08932000((char *)self + 0x40, self->a, self->b, n);
}
