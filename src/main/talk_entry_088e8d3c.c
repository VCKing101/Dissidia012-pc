/* Constructor: sets up the pool at +4 (func_08864FF0), sub-objects at 0x90, 0x1874 and 0x1AF0, clears its state fields, then calls func_088E9228. */
typedef struct Obj088e8d3c {
    int f0;
    unsigned char pool[0x34];
    int f38;
    unsigned char b3c;
    unsigned char pad3d[0x90 - 0x3D];
    unsigned char sub0[0x184A - 0x90];
    unsigned char b184a;
    unsigned char pad184b[0x1860 - 0x184B];
    int f1860;
    int f1864;
    unsigned char pad1868[0x1B04 - 0x1868];
    unsigned char b1b04, b1b05, b1b06;
} Obj088e8d3c;
extern void func_08864FF0(void *p, int a, int b, int c);
extern void func_088DFCF8(void *p);
extern void func_088D6C9C(void *p);
extern void func_088CE334(void *p);
extern void func_088E9228(void *p);
Obj088e8d3c *func_088E8D3C(Obj088e8d3c *self)
{
    func_08864FF0(self->pool, 1, 0x11171, 0x54);
    func_088DFCF8((char *)self + 0x90);
    func_088D6C9C((char *)self + 0x1874);
    func_088CE334((char *)self + 0x1AF0);
    self->f0 = 0;
    self->b3c = 0;
    self->f38 = 0;
    self->b1b05 = 0;
    self->b1b04 = 0;
    self->f1860 = 0;
    self->f1864 = 4;
    self->b1b06 = 0;
    self->b184a = 1;
    func_088E9228(self);
    return self;
}
