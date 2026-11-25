typedef struct Obj { int a0; int a4; int a8; int h; } Obj;
extern int D_089D7830;
extern void func_088D6DBC(Obj *o);
extern int func_088CF188(int g, int a, int b, int c, int d, int e, int f);
extern int func_088D7720(Obj *o, int a, int b, int c, int d, int e, int f);
int func_088D6F24(Obj *o, int p1, int p2, int p3, int p5, int p6, int p7, int p8, int x0, int x4, int x8)
{
    int g;
    int r;
    func_088D6DBC(o);
    if (p1 == 0) return -1;
    g = D_089D7830;
    o->h = func_088CF188(g, p6, p7, p8, x0, x4, x8);
    if (o->h == 0) return -2;
    r = func_088D7720(o, p1, p2, p3, p5, 0, 0);
    if (r != 0) return -3;
    return 0;
}
