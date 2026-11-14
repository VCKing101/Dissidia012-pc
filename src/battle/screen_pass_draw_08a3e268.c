/* Draws the pending full-screen pass: a 480x272 viewport with blending set
 * up, drawn once and cleared. */
typedef struct ScreenRect {
    short x;
    short y;
    short z;
    short width;
    short height;
    short pad;
} ScreenRect;

typedef struct ScreenPass {
    unsigned char pad_000[0x280];
    int pending;
} ScreenPass;

extern ScreenRect *func_089A00F4(int size);
extern void func_0899FDC4(int mode);
extern void func_089A12A4(int a, int b, int c, int d, int e);
extern void func_089A03DC(int state);
extern void func_089A0C8C(int color);
extern void func_089A0520(int prim, int type, int count, int index, ScreenRect *vertices);

void func_08A3E268(ScreenPass *self)
{
    ScreenRect *rect;
    if (self->pending == 0) {
        return;
    }
    self->pending = 0;
    rect = func_089A00F4(0xC);
    rect->z = 0;
    rect->y = 0;
    rect->x = 0;
    rect->width = 480;
    rect->height = 272;
    rect->pad = 0;
    func_0899FDC4(0);
    func_089A12A4(0, 2, 3, 0, 0);
    func_089A03DC(1);
    func_089A03DC(4);
    func_089A03DC(9);
    func_089A0C8C(0xFFFFFF);
    func_089A0520(6, 0x800100, 2, 0, rect);
}
