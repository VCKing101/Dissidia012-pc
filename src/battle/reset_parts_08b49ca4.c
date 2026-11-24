// Unless the byte at +0x6F0 is set, calls func_0892AE68 on the three sub-objects at +4, +0x25C and +0x308.
extern void func_0892AE68(void *);
typedef struct { char pad0[4]; char a[0x25C - 4]; char b[0x308 - 0x25C]; char c[1]; } Obj08B49CA4;
void func_08B49CA4(unsigned char *self)
{
    if (self[0x6F0] != 0) {
        return;
    }
    func_0892AE68((char *)self + 4);
    func_0892AE68((char *)self + 0x25C);
    func_0892AE68((char *)self + 0x308);
}
