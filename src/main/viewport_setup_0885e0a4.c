typedef struct Viewport { short x; short y; short field_04; short w; short h; short field_0A; } Viewport;
extern Viewport *func_089A00F4(int size);
extern int func_089A04C4(int id);
extern void func_089A03DC(int id);
extern void func_089A0C8C(int arg);
extern void func_089A0520(int a, int b, int c, int d, Viewport *vp);
extern void func_089A0438(int id, int value);
void func_0885E0A4(int unused, int w, int h, int arg)
{
    Viewport *vp = func_089A00F4(12);
    int saved;
    vp->x = 0;
    vp->y = 0;
    vp->field_04 = -1;
    vp->w = w;
    vp->h = h;
    vp->field_0A = -1;
    saved = func_089A04C4(9);
    func_089A03DC(9);
    func_089A0C8C(arg);
    func_089A0520(6, 0x800100, 2, 0, vp);
    func_089A0438(9, saved);
}
