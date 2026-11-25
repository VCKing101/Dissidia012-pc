/* Sets up and submits a 480x272 screen-sized fill: allocates the two command blocks, fills the rectangle and sets the colour from the owner. */
typedef struct Size { short w, h, d; } Size;
typedef struct Rect { short x, y, z; Size s; } Rect;
typedef struct S { char p0[0xC]; int colour; } S;
extern Rect *func_089A00F4(int n);
extern void *func_089A04E8(void);
extern void func_089A12A4(int a, int b, int c, int d, int e);
extern void func_089A03DC(int v);
extern void func_089A0384(int v);
extern void func_089A0C8C(int v);
extern void func_089A0520(int a, int b, int c, int d, Rect *r);
extern void func_0899FDC4(void *p);
void func_088B18D0(S *self)
{
    Rect *r = func_089A00F4(0xC);
    void *blk = func_089A04E8();
    Size *sz;
    r->x = 0;
    r->y = 0;
    r->z = 0;
    sz = &r->s;
    sz->w = 0x1E0;
    sz->h = 0x110;
    sz->d = 0;
    func_089A12A4(0, 2, 3, 0, 0);
    func_089A03DC(1);
    func_089A0384(4);
    func_089A03DC(9);
    func_089A0C8C(self->colour << 24);
    func_089A0520(6, 0x800100, 2, 0, r);
    func_0899FDC4(blk);
}
