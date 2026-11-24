// Constructor-style init of an effect object: runs sub-initialisers at +0, 0xC0, 0x750, 0x878, 0x8D0, 0x990 and clears 0x20 bytes at 0x6D0.
extern void func_0885E7C0(void *);
extern void func_08873498(void *);
extern void func_0880FB6C(void *);
extern void func_08B41A54(void *);
extern void func_08935828(void *, int, int);
typedef struct Eff57 {
    char pad0[0x870];
    int f870;
    float f874;
    char pad1[0x8BC - 0x878];
    unsigned char f8BC;
    char pad2[3];
    int f8C0;
} Eff57;
Eff57 *func_08B3B9C4(Eff57 *self)
{
    func_0885E7C0(self);
    func_08873498((char *)self + 0xC0);
    func_0880FB6C((char *)self + 0x750);
    self->f870 = 0;
    self->f874 = 2.0f;
    func_08B41A54((char *)self + 0x878);
    self->f8BC = 1;
    self->f8C0 = 0;
    func_0885E7C0((char *)self + 0x8D0);
    func_08873498((char *)self + 0x990);
    func_08935828((char *)self + 0x6D0, 0, 0x20);
    return self;
}
